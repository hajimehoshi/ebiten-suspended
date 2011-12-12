#ifndef EBITEN_KERNELS_KERNEL_HPP
#define EBITEN_KERNELS_KERNEL_HPP

#include "ebiten/platform.hpp"

#ifdef EBITEN_MACOSX
# include "ebiten/kernels/detail/macosx/application.hpp"
#endif

#include "ebiten/graphics/device.hpp"
#include "ebiten/noncopyable.hpp"
#include "ebiten/timers/timer.hpp"
#include <functional>

namespace ebiten {
namespace kernels {

class kernel : private ebiten::noncopyable {
private:
  std::function<void(graphics::texture_factory&)> game_update_;
  std::function<void(graphics::graphics_context&)> game_draw_;
  std::size_t const screen_width_;
  std::size_t const screen_height_;
  std::size_t const screen_scale_;
  std::size_t const fps_;
  uint64_t before_;
  std::unique_ptr<graphics::device> device_;
public:
  kernel(std::function<void(graphics::texture_factory&)> game_update,
         std::function<void(graphics::graphics_context&)> game_draw,
         std::size_t screen_width,
         std::size_t screen_height,
         std::size_t screen_scale,
         std::size_t fps)
    : game_update_(game_update),
      game_draw_(game_draw),
      screen_width_(screen_width),
      screen_height_(screen_height),
      screen_scale_(screen_scale), // TODO: check the scale (1 or 2?) not to crash
      fps_(fps),
      before_(timers::timer::now_nsec() * fps) {
  }
  // TODO: Refactoring
#ifndef EBITEN_IOS
  void
  main_loop() {
    frames::frame frame(this->screen_width_  * this->screen_scale_,
                        this->screen_height_ * this->screen_scale_);
    this->device_.reset(new graphics::device(this->screen_width_,
                                             this->screen_height_,
                                             this->screen_scale_,
                                             frame,
                                             std::bind(&kernel::update,
                                                       this,
                                                       std::placeholders::_1),
                                             std::bind(&kernel::draw,
                                                       this,
                                                       std::placeholders::_1)));
    detail::run_application(frame);
  }
#endif
  template<class View>
  void
  start_with_view(View& view) {
    this->device_.reset(new graphics::device(this->screen_width_,
                                             this->screen_height_,
                                             this->screen_scale_,
                                             view,
                                             std::bind(&kernel::update,
                                                       this,
                                                       std::placeholders::_1),
                                             std::bind(&kernel::draw,
                                                       this,
                                                       std::placeholders::_1)));
  }
private:
  void
  update(graphics::device& device) {
    uint64_t const now = timers::timer::now_nsec() * this->fps_;
    while (this->before_ + 1000 * 1000 * 1000 < now) {
      this->game_update_(device.texture_factory());
      this->before_ += 1000 * 1000 * 1000;
    }
  }
  void
  draw(graphics::device& device) {
    this->game_draw_(device.graphics_context());
  }
};

}
}

#endif
