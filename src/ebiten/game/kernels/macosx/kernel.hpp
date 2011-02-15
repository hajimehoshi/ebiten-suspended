#ifndef EBITEN_GAME_KERNELS_MACOSX_KERNEL_HPP
#define EBITEN_GAME_KERNELS_MACOSX_KERNEL_HPP

#include "ebiten/game/frames/cocoa/frame.hpp"
#include "ebiten/game/graphics/opengl/device.hpp"
#include "ebiten/game/graphics/opengl/cocoa/view.hpp"
#include "ebiten/game/kernels/macosx/application.hpp"
#include "ebiten/game/timers/mach/timer.hpp"
#include "ebiten/util/singleton.hpp"
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/optional.hpp>
#include <boost/range.hpp>
#include <boost/type_traits.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/utility/in_place_factory.hpp>
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

    std::size_t const frame_width  = screen_width * window_scale;
    std::size_t const frame_height = screen_height * window_scale;
    frames::cocoa::frame frame(frame_width, frame_height);
    struct draw_sprites_func {
      static void
      invoke(pthread_mutex_t& mutex, Game const& game) {
        lock l(mutex);
        typedef BOOST_TYPEOF(game.sprites()) sprites_type;
        sprites_type const& sprites = game.sprites();
        typedef boost::shared_ptr<graphics::sprite> sprites_element_type;
        BOOST_STATIC_ASSERT((boost::is_same<typename boost::range_value<sprites_type>::type,
                             sprites_element_type>::value));
        // TODO: use reference_wrapper?
        std::vector<sprites_element_type> sorted_sprites;
        sorted_sprites.reserve(boost::size(sprites));
        BOOST_FOREACH(sprites_element_type const& s, sprites) {
          sorted_sprites.push_back(s);
        };
        // sort the sprites in desceinding order of z
        struct sprites_cmp {
          static int
          invoke(sprites_element_type const& a,
                 sprites_element_type const& b) {
            const double diff = a->z() - b->z();
            return (0 < diff) ? -1 : ((diff < 0) ? 1 : 0);
          }
        };
        std::sort(sorted_sprites.begin(), sorted_sprites.end(),
                  sprites_cmp::invoke);
        BOOST_FOREACH(sprites_element_type const& s, sorted_sprites) {
          s->draw(graphics::opengl::graphics_context::instance());
        };
      }
    };
    boost::function<void()> draw_sprites = boost::bind(&draw_sprites_func::invoke,
                                                       boost::ref(mutex),
                                                       boost::ref(game));
    struct update_device_func {
      static void
      invoke(boost::optional<graphics::opengl::device>& device,
             boost::function<void()>& draw_sprites) {
        device->update(draw_sprites);
      }
    };
    boost::optional<graphics::opengl::device> device;
    boost::function<void()> update_device = boost::bind(&update_device_func::invoke,
                                                        boost::ref(device),
                                                        boost::ref(draw_sprites));
    graphics::opengl::cocoa::view<BOOST_TYPEOF(frame)> view(frame, update_device);
    device = boost::in_place(screen_width, screen_height, window_scale);
    // TODO: remove initialize...
    game.initialize(device->texture_factory());

    // start the logic loop
    struct logic_func {
      static void
      invoke(std::size_t fps, pthread_mutex_t& mutex, Game& game) {
        int frame_count = 0;
        timers::mach::timer timer(fps);
        for (;;) {
          lock l(mutex);
          timer.wait_frame();
          game.update(frame_count);
          ++frame_count;
        }
      }
    };
    boost::function<void()> logic = boost::bind(logic_func::invoke,
                                                fps,
                                                boost::ref(mutex),
                                                boost::ref(game));
    struct logic_func_wrapper {
      static void*
      invoke(void* func_p) {
        typedef boost::function<void*()> func_type;
        func_type func = *(reinterpret_cast<func_type*>(func_p));
        func();
        return 0;
      }
    };
    pthread_t logic_thread;
    ::pthread_create(&logic_thread, 0, logic_func_wrapper::invoke, &logic);
    application::instance().run(frame);
    //game_terminated.store(true);
    //::pthread_join(logic_thread, 0);
  }
};

}
}
}
}

#endif
