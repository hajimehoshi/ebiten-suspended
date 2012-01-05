#ifndef EBITEN_KERNELS_KERNEL_HPP
#define EBITEN_KERNELS_KERNEL_HPP

#include "ebiten/graphics/device.hpp"
#include "ebiten/graphics/graphics_context.hpp"
#include "ebiten/graphics/native_view.hpp"
#include "ebiten/graphics/texture_factory.hpp"
#include "ebiten/noncopyable.hpp"
#include "ebiten/timers/timer.hpp"
#include <functional>

namespace ebiten {
namespace kernels {

class kernel : private noncopyable {
private:
  std::function<bool(graphics::texture_factory&)> game_update_;
  std::function<void(graphics::graphics_context&, graphics::texture&)> game_draw_;
  std::size_t const fps_;
  uint64_t before_;
  graphics::device device_;
  bool is_terminated_;
public:
  kernel(std::function<bool(graphics::texture_factory&)> game_update,
         std::function<void(graphics::graphics_context&, graphics::texture&)> game_draw,
         std::size_t screen_width,
         std::size_t screen_height,
         std::size_t screen_scale, // TODO: check the scale (1 or 2?) not to crash
         std::size_t fps,
         graphics::native_view native_view)
    : game_update_(game_update),
      game_draw_(game_draw),
      fps_(fps),
      before_(timers::timer::now_nsec() * fps),
      device_(screen_width,
              screen_height,
              screen_scale,
              native_view,
              std::bind(&kernel::update,
                        this,
                        std::placeholders::_1),
              std::bind(&kernel::draw,
                        this,
                        std::placeholders::_1,
                        std::placeholders::_2)),
      is_terminated_(false) {
  }
  bool
  is_terminated() const {
    return this->is_terminated_;
  }
private:
  bool
  update(ebiten::graphics::texture_factory& tf) {
    if (this->is_terminated_) {
      return true;
    }
    uint64_t const now = timers::timer::now_nsec() * this->fps_;
    while (this->before_ + 1000 * 1000 * 1000 < now) {
      bool const terminated = this->game_update_(tf);
      if (terminated) {
        this->is_terminated_ = true;
        return true;
      }
      this->before_ += 1000 * 1000 * 1000;
    }
    return false;
  }
  void
  draw(ebiten::graphics::graphics_context& g,
       ebiten::graphics::texture& offscreen_texture) {
    if (this->is_terminated_) {
      return;
    }
    this->game_draw_(g, offscreen_texture);
  }
};

}
}

#endif
