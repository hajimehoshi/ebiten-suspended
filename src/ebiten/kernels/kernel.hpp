#ifndef EBITEN_KERNELS_KERNEL_HPP
#define EBITEN_KERNELS_KERNEL_HPP

#include "ebiten/platform.hpp"

#ifdef EBITEN_MACOSX
#include "ebiten/kernels/detail/macosx/application.hpp"
#endif

#include "ebiten/graphics/device.hpp"
#include "ebiten/timers/timer.hpp"
#include <functional>

namespace ebiten {
namespace kernels {

template<class Game>
void
run(Game& game,
    std::size_t screen_width,
    std::size_t screen_height,
    std::size_t fps,
    std::size_t screen_scale) {
  struct update_func {
    static void
    invoke(std::size_t fps,
           Game& game,
           graphics::device& device) {
      (void)fps;
      // TODO: Adjust FPS
      game.update(device.texture_factory());
    }
  };
  struct draw_func {
    static void
    invoke(Game const& game,
           graphics::device& device) {
      game.draw(device.graphics_context());
    }
  };
  frames::frame frame(screen_width * screen_scale, screen_height * screen_scale);
  graphics::device device(screen_width,
                          screen_height,
                          screen_scale,
                          frame,
                          std::bind(update_func::invoke,
                                    fps,
                                    std::ref(game),
                                    std::placeholders::_1),
                          std::bind(draw_func::invoke,
                                    std::cref(game),
                                    std::placeholders::_1));
  detail::run_application(frame);
}

}
}

#endif
