#ifndef EBITEN_GAME_KERNELS_MACOSX_KERNEL_HPP
#define EBITEN_GAME_KERNELS_MACOSX_KERNEL_HPP

#include "ebiten/game/frames/cocoa/frame.hpp"
#include "ebiten/game/graphics/opengl/device.hpp"
#include "ebiten/game/graphics/opengl/cocoa/view.hpp"
#include "ebiten/game/kernels/macosx/application.hpp"
#include "ebiten/game/timers/mach/timer.hpp"
#include "ebiten/util/singleton.hpp"
#include <boost/optional.hpp>
#include <boost/range.hpp>
#include <boost/utility/in_place_factory.hpp>
#include <atomic>
#include <pthread.h>

namespace ebiten {

namespace game {
namespace kernels {
namespace macosx {

class kernel : public util::singleton<kernel> {
  friend class util::singleton<kernel>;
public:
  template<class Game>
  void
  run(Game& game,
      std::size_t screen_width,
      std::size_t screen_height,
      std::size_t fps,
      std::size_t window_scale) {
    class lock {
    public:
      lock(pthread_mutex_t& mutex)
        : mutex_(mutex) {
        ::pthread_mutex_lock(&this->mutex_);
      }
      ~lock() {
        ::pthread_mutex_unlock(&this->mutex_);
      }
    private:
      pthread_mutex_t& mutex_;
    };
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    frames::cocoa::frame frame(screen_width * window_scale,
                               screen_height * window_scale);
    std::function<void()> draw_sprites = [&]{
      lock l(mutex);
      const auto& sprites = game.sprites();
      typedef graphics::sprite sprite;
      typedef std::reference_wrapper<const sprite> sprite_cref;
      std::vector<sprite_cref> sorted_sprites;
      sorted_sprites.reserve(boost::size(sprites));
      std::for_each(boost::begin(sprites), boost::end(sprites),
                    [&](const sprite& s) {
                      sorted_sprites.emplace_back(s);
                    });
      // sort the sprites in desceinding order of z
      std::sort(sorted_sprites.begin(), sorted_sprites.end(),
                [](const sprite_cref& a, const sprite_cref& b) {
                  const double diff = a.get().z() - b.get().z();
                  return (0 < diff) ? -1 : ((diff < 0) ? 1 : 0);
                });
      std::for_each(sorted_sprites.begin(), sorted_sprites.end(),
                    [](const sprite_cref& s) {
                      s.get().draw(graphics::opengl::graphics_context::instance());
                    });
    };

    boost::optional<graphics::opengl::device> device;
    graphics::opengl::cocoa::view<decltype(frame)> view(frame,
                                                        screen_width * window_scale,
                                                        screen_height * window_scale,
                                                        [&]{
                                                          device->update(draw_sprites);
                                                        });
    device = boost::in_place(screen_width, screen_height, window_scale);
    // TODO: remove initialize...
    game.initialize(device->texture_factory());

    // start the logic loop
    struct logic_func_struct {
      static void* invoke(void* func_ptr) {
        auto func = *(reinterpret_cast<std::function<void()>*>(func_ptr));
        func();
        return nullptr;
      }
    };
    std::atomic<bool> game_terminated(false);
    std::function<void()> logic_func = [&]{
      int frame_count = 0;
      timers::mach::timer timer(fps);
      while (!game_terminated.load()) {
        lock l(mutex);
        timer.wait_frame();
        game.update(frame_count);
        ++frame_count;
      }
    };
    pthread_t logic_thread;
    ::pthread_create(&logic_thread, nullptr, logic_func_struct::invoke, &logic_func);
    application::instance().run(frame);
    game_terminated.store(true);
    ::pthread_join(logic_thread, nullptr);
  }
};

}
}
}
}

#endif
