#ifndef EBITEN_KERNEL_KERNEL_HPP
#define EBITEN_KERNEL_KERNEL_HPP

#include "ebiten/platform.hpp"

#ifdef EBITEN_MACOSX
#include "ebiten/kernel/detail/macosx/application.hpp"
#endif

#include "ebiten/graphics/device.hpp"
#include "ebiten/graphics/sprite.hpp"
#include "ebiten/timers/timer.hpp"
#include <algorithm>
#include <functional>
#include <pthread.h>

namespace ebiten {
namespace kernel {

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

  struct draw_func {
    static void
    invoke(pthread_mutex_t& mutex,
           Game const& game,
           graphics::device& device) {
      lock l(mutex);
      game.draw(device.graphics_context());
    }
  };
  frames::frame frame(screen_width * window_scale, screen_height * window_scale);
  graphics::device device(screen_width,
                          screen_height,
                          window_scale,
                          frame,
                          std::bind(&draw_func::invoke,
                                    std::ref(mutex),
                                    std::cref(game),
                                    std::placeholders::_1));
  game.initialize(device.texture_factory());
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
                                          std::ref(game));
  struct logic_func_wrapper {
    static void*
    invoke(void* func_p) {
      typedef std::function<void()> func_type;
      func_type& func = *(reinterpret_cast<func_type*>(func_p));
      func();
      return nullptr;
    }
  };
  pthread_t logic_thread;
  ::pthread_create(&logic_thread, 0, logic_func_wrapper::invoke, &logic);
  detail::run_application(frame);
  //game_terminated.store(true);
  //::pthread_join(logic_thread, 0);
}

}
}

#endif
