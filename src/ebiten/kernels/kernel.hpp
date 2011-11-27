#ifndef EBITEN_KERNELS_KERNEL_HPP
#define EBITEN_KERNELS_KERNEL_HPP

#include "ebiten/platform.hpp"

#ifdef EBITEN_MACOSX
#include "ebiten/kernels/detail/macosx/application.hpp"
#endif

#include "ebiten/graphics/device.hpp"
#include "ebiten/graphics/sprite.hpp"
#include "ebiten/timers/timer.hpp"
#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>
#include <boost/utility/in_place_factory.hpp>
#include <algorithm>
#include <functional>
#include <pthread.h>

namespace ebiten {
namespace kernels {

class kernel : private boost::noncopyable {
public:
  template<class Game>
  void
  run(std::size_t screen_width,
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

    struct draw_sprites_func {
      static void
      invoke(pthread_mutex_t& mutex,
             boost::optional<Game> const& game,
             graphics::device& device) {
        lock l(mutex);
        auto const& sprites = game->sprites();
        typedef std::reference_wrapper<graphics::sprite const> sprite_cref;
        std::vector<sprite_cref> sorted_sprites;
        sorted_sprites.reserve(boost::size(sprites));
        for (auto const& s : sprites) {
          sorted_sprites.push_back(std::ref(s));
        };
        // sort the sprites in desceinding order of z
        struct sprites_cmp {
          static int
          invoke(sprite_cref const& a, sprite_cref const& b) {
            double const diff = a.get().z() - b.get().z();
            return (0 < diff) ? -1 : ((diff < 0) ? 1 : 0);
          }
        };
        std::sort(sorted_sprites.begin(), sorted_sprites.end(),
                  sprites_cmp::invoke);
        for (auto const& s : sorted_sprites) {
          s.get().draw(device.graphics_context());
        };
      }
    };
    boost::optional<Game> game;
    graphics::device device(screen_width,
                            screen_height,
                            window_scale);
    device.connect_drawing_sprites(std::bind(&draw_sprites_func::invoke,
                                             std::ref(mutex),
                                             std::cref(game),
                                             std::ref(device)));
    game = boost::in_place(std::ref(device.texture_factory()));
    // start the logic loop
    struct logic_func {
      static void
      invoke(std::size_t fps, pthread_mutex_t& mutex, Game& game) {
        int frame_count = 0;
        timers::timer timer(fps);
        for (;;) {
          timer.wait_frame();
          lock l(mutex);
          game.update(frame_count);
          ++frame_count;
        }
      }
    };
    std::function<void()> logic = std::bind(logic_func::invoke,
                                            fps,
                                            std::ref(mutex),
                                            std::ref(*game));
    struct logic_func_wrapper {
      static void*
      invoke(void* func_p) {
        typedef std::function<void()> func_type;
        func_type& func = *(reinterpret_cast<func_type*>(func_p));
        func();
        return 0;
      }
    };
    pthread_t logic_thread;
    ::pthread_create(&logic_thread, 0, logic_func_wrapper::invoke, &logic);
    detail::application app(device.frame());
    //game_terminated.store(true);
    //::pthread_join(logic_thread, 0);
  }
};

}
}

#endif
