#ifndef EBITEN_KERNELS_KERNEL_HPP
#define EBITEN_KERNELS_KERNEL_HPP

#include "ebiten/platform.hpp"

#ifdef EBITEN_MACOSX
# include "ebiten/kernels/detail/macosx/application.hpp"
#endif

#include "ebiten/graphics/device.hpp"
#include "ebiten/graphics/view.hpp"
#include "ebiten/noncopyable.hpp"
#include "ebiten/timers/timer.hpp"
#include <functional>

namespace ebiten {
namespace kernels {

class kernel : private ebiten::noncopyable {
private:
  std::function<void(graphics::texture_factory&)> game_update_;
  std::function<void(graphics::graphics_context&)> game_draw_;
  std::size_t const fps_;
  graphics::view& view_;
  uint64_t before_;
  graphics::device device_;
public:
  kernel(std::function<void(graphics::texture_factory&)> game_update,
         std::function<void(graphics::graphics_context&)> game_draw,
         std::size_t screen_width,
         std::size_t screen_height,
         std::size_t screen_scale, // TODO: check the scale (1 or 2?) not to crash
         std::size_t fps,
         graphics::view& view)
    : game_update_(game_update),
      game_draw_(game_draw),
      fps_(fps),
      view_(view),
      before_(timers::timer::now_nsec() * fps),
      device_(screen_width,
              screen_height,
              screen_scale,
              view,
              std::bind(&kernel::update, this),
              std::bind(&kernel::draw, this)) {
  }
  void
  run_main_loop() {
    detail::run_application(this->view_);
  }
private:
  void
  update() {
    uint64_t const now = timers::timer::now_nsec() * this->fps_;
    while (this->before_ + 1000 * 1000 * 1000 < now) {
      this->game_update_(this->device_.texture_factory());
      this->before_ += 1000 * 1000 * 1000;
    }
  }
  void
  draw() {
    this->game_draw_(this->device_.graphics_context());
  }
};

}
}

#endif
