#ifndef EBITEN_GAME_KERNELS_DETAIL_KERNEL_HPP
#define EBITEN_GAME_KERNELS_DETAIL_KERNEL_HPP

#include "ebiten/game/graphics/sprite.hpp"
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>
#include <boost/range.hpp>
#include <boost/type_traits.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/utility/in_place_factory.hpp>
#include <algorithm>
#include <pthread.h>

namespace ebiten {
namespace game {
namespace kernels {
namespace detail {

template<class Device, class Timer, class Application>
class kernel : boost::noncopyable {
public:
  typedef Device device_type;
  typedef Timer timer_type;
  typedef Application application_type;
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
             Device& device) {
        lock l(mutex);
        BOOST_AUTO(const& sprites, game->sprites());
        typedef boost::reference_wrapper<graphics::sprite const> sprite_cref;
        std::vector<sprite_cref> sorted_sprites;
        sorted_sprites.reserve(boost::size(sprites));
        BOOST_FOREACH(graphics::sprite const& s, sprites) {
          sorted_sprites.push_back(boost::ref(s));
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
        BOOST_FOREACH(sprite_cref const& s, sorted_sprites) {
          s.get().draw(device.graphics_context());
        };
      }
    };
    boost::optional<Game> game;
    Device device(screen_width,
                  screen_height,
                  window_scale);
    device.connect_updating(boost::bind(&draw_sprites_func::invoke,
                                        boost::ref(mutex),
                                        boost::cref(game),
                                        boost::ref(device)));
    game = boost::in_place(boost::ref(device.texture_factory()));
    // start the logic loop
    struct logic_func {
      static void
      invoke(std::size_t fps, pthread_mutex_t& mutex, Game& game) {
        int frame_count = 0;
        Timer timer(fps);
        for (;;) {
          timer.wait_frame();
          lock l(mutex);
          game.update(frame_count);
          ++frame_count;
        }
      }
    };
    boost::function<void()> logic = boost::bind(logic_func::invoke,
                                                fps,
                                                boost::ref(mutex),
                                                boost::ref(*game));
    struct logic_func_wrapper {
      static void*
      invoke(void* func_p) {
        typedef boost::function<void()> func_type;
        func_type& func = *(reinterpret_cast<func_type*>(func_p));
        func();
        return 0;
      }
    };
    pthread_t logic_thread;
    ::pthread_create(&logic_thread, 0, logic_func_wrapper::invoke, &logic);
    Application application(device);
    //game_terminated.store(true);
    //::pthread_join(logic_thread, 0);
  }
};

}
}
}
}

#endif

