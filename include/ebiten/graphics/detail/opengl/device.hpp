#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_DEVICE_HPP
#define EBITEN_GRAPHICS_DETAIL_OPENGL_DEVICE_HPP

#include "ebiten/graphics/detail/opengl/graphics_context.hpp"
#include "ebiten/graphics/detail/opengl/opengl_initializer.hpp"
#include "ebiten/graphics/detail/opengl/texture_factory.hpp"
#include "ebiten/graphics/native_view.hpp"
#include "ebiten/noncopyable.hpp"
#include "ebiten/platform.hpp"

#ifdef EBITEN_MACOSX
# include <OpenGL/gl.h>
#endif
#ifdef EBITEN_IOS
# import <GLKit/GLKit.h>
#endif

#include <cassert>
#include <functional>

namespace ebiten {
namespace graphics {
namespace detail {

class device : private noncopyable {
private:
  std::size_t const screen_width_;
  std::size_t const screen_height_;
  std::size_t const screen_scale_;
  std::function<void()> update_func_;
  std::function<void()> draw_func_;
  texture_factory texture_factory_;
  graphics_context graphics_context_;
  texture offscreen_texture_;
  opengl_initializer opengl_initializer_;
public:
  device(std::size_t screen_width,
         std::size_t screen_height,
         std::size_t screen_scale,
         native_view native_view,
         std::function<void()> const& update_func,
         std::function<void()> const& draw_func)
    : screen_width_(screen_width),
      screen_height_(screen_height),
      screen_scale_(screen_scale),
      update_func_(update_func),
      draw_func_(draw_func),
      texture_factory_(),
      graphics_context_(this->texture_factory_) {
    assert(0 < this->screen_width_);
    assert(0 < this->screen_height_);
    assert(0 < this->screen_scale_);
    assert(this->update_func_);
    assert(this->draw_func_);
    this->opengl_initializer_.initialize(native_view, std::bind(&device::update, this));
  }
  // TODO: destructor
  /*
   * NOTICE:
   *   The OpenGL functions should be called only in this method 'update'.
   *   Is that better to add an argument to this method?
   */
  void
  update() {
    // TODO: If application is terminated, stop
    if (!this->offscreen_texture_) {
      this->offscreen_texture_ = this->texture_factory_.create(this->screen_width_,
                                                                   this->screen_height_);
      assert(static_cast<bool>(this->offscreen_texture_));
    }
    this->update_func_();
    detail::graphics_context& g = this->graphics_context_;
    ::glEnable(GL_TEXTURE_2D);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    g.set_offscreen(this->offscreen_texture_,
                    0, static_cast<float>(this->offscreen_texture_.texture_width()),
                    0, static_cast<float>(this->offscreen_texture_.texture_height()));
    this->draw_func_();
    g.flush();
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    std::size_t const width  = this->screen_width_  * this->screen_scale_;
    std::size_t const height = this->screen_height_ * this->screen_scale_;
    g.set_offscreen(graphics::texture(),
                    0, static_cast<float>(width),
                    static_cast<float>(height), 0);
    g.set_texture(this->offscreen_texture_);
    g.draw(0, 0, this->screen_width_, this->screen_height_,
           0, 0,
           this->screen_width_  * this->screen_scale_,
           this->screen_height_ * this->screen_scale_);
    g.flush();
  }
  graphics_context&
  graphics_context() {
    return this->graphics_context_;
  }
  texture_factory&
  texture_factory() {
    return this->texture_factory_;
  }
};

}
}
}

#endif
