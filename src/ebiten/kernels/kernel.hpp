#ifndef EBITEN_KERNELS_KERNEL_HPP
#define EBITEN_KERNELS_KERNEL_HPP

#include "ebiten/platform.hpp"

#ifdef EBITEN_MACOSX
#include "ebiten/kernels/detail/macosx/application.hpp"
#endif

#include "ebiten/graphics/device.hpp"
#include "ebiten/noncopyable.hpp"
#include "ebiten/timers/timer.hpp"
#include <functional>

namespace ebiten {
namespace kernels {

class kernel : private ebiten::noncopyable {
public:
  kernel() {
  }
  template<class Game>
  void
  run(Game& game,
      std::size_t screen_width,
      std::size_t screen_height,
      std::size_t fps,
      std::size_t screen_scale) {
    class update_func_t {
    private:
      std::size_t const fps_;
      Game& game_;
      uint64_t before_;
    public:
      update_func_t(std::size_t fps, Game& game)
        : fps_(fps),
          game_(game),
          before_(timers::timer::now_nsec() * fps) {
      }
      void
      invoke(graphics::device& device) {
        uint64_t const now = timers::timer::now_nsec() * this->fps_;
        while (this->before_ + 1000 * 1000 * 1000 < now) {
          this->game_.update(device.texture_factory());
          this->before_ += 1000 * 1000 * 1000;
        }
      }
    } update_func(fps, game);
    class draw_func_t {
    private:
      Game& game_;
    public:
      draw_func_t(Game& game)
        : game_(game) {
      }
      void
      invoke(graphics::device& device) {
        this->game_.draw(device.graphics_context());
      }
    } draw_func(game);
    frames::frame frame(screen_width * screen_scale, screen_height * screen_scale);
    graphics::device device(screen_width,
                            screen_height,
                            screen_scale,
                            frame,
                            std::bind(&update_func_t::invoke,
                                      update_func,
                                      std::placeholders::_1),
                            std::bind(&draw_func_t::invoke,
                                      draw_func,
                                      std::placeholders::_1));
    detail::run_application(frame);
  }
};

}
}

#endif
