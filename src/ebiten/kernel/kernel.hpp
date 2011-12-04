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
#include <thread>

namespace ebiten {
namespace kernel {

template<class Game>
void
run(Game& game,
    std::size_t screen_width,
    std::size_t screen_height,
    std::size_t fps,
    std::size_t screen_scale) {
  std::mutex mutex;

  struct draw_func {
    static void
    invoke(std::mutex& mutex,
           Game const& game,
           graphics::device& device) {
      
      {
        std::lock_guard<std::mutex> lock(mutex);
        game.draw(device.graphics_context());
      }
    }
  };
  frames::frame frame(screen_width * screen_scale, screen_height * screen_scale);
  graphics::device device(screen_width,
                          screen_height,
                          screen_scale,
                          frame,
                          std::bind(&draw_func::invoke,
                                    std::ref(mutex),
                                    std::cref(game),
                                    std::placeholders::_1));
  game.initialize(device.texture_factory());
  // start the logic loop
  struct logic_func {
    static void
    invoke(std::size_t fps, std::mutex& mutex, Game& game) {
      int frame_count = 0;
      timers::timer timer(fps);
      for (;;) {
        timer.wait_frame();
        {
          std::lock_guard<std::mutex> lock(mutex);
          game.update(frame_count);
        }
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
  std::thread logic_thread(logic_func_wrapper::invoke, &logic);
  detail::run_application(frame);
}

}
}

#endif
