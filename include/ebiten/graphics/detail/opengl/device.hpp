#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_DEVICE_HPP
#define EBITEN_GRAPHICS_DETAIL_OPENGL_DEVICE_HPP

#include "ebiten/graphics/detail/opengl/graphics_context.hpp"
#include "ebiten/graphics/detail/opengl/opengl.hpp"
#include "ebiten/graphics/detail/opengl/opengl_initializer.hpp"
#include "ebiten/graphics/detail/opengl/texture_factory.hpp"
#include "ebiten/graphics/detail/opengl/texture_pointer.hpp"
#include "ebiten/graphics/native_view.hpp"
#include "ebiten/noncopyable.hpp"
#include <cassert>
#include <functional>
#include <memory>

namespace ebiten {
namespace graphics {
namespace detail {

class device : private noncopyable {
private:
  std::size_t screen_width_;
  std::size_t screen_height_;
  std::size_t const screen_scale_;
  std::function<bool(texture_factory&)> update_func_;
  std::function<void(graphics_context&, texture&)> draw_func_;
  texture_factory texture_factory_;
  graphics_context graphics_context_;
  texture_pointer offscreen_texture_;
  opengl_initializer opengl_initializer_;
  bool to_destory_offscreen_texture_;
public:
  device(std::size_t screen_width,
         std::size_t screen_height,
         std::size_t screen_scale,
         native_view native_view,
         std::function<bool(texture_factory&)> const& update_func,
         std::function<void(graphics_context&, texture&)> const& draw_func)
    : screen_width_(screen_width),
      screen_height_(screen_height),
      screen_scale_(screen_scale),
      update_func_(update_func),
      draw_func_(draw_func),
      texture_factory_(),
      graphics_context_(screen_width,
                        screen_height,
                        screen_scale,
                        this->texture_factory_),
      opengl_initializer_(native_view),
      to_destory_offscreen_texture_(false) {
    assert(0 < this->screen_width_);
    assert(0 < this->screen_height_);
    assert(0 < this->screen_scale_);
    assert(this->screen_width_  <= 4096);
    assert(this->screen_height_ <= 4096);
    assert(this->screen_scale_  <= 4);
    assert(this->update_func_);
    assert(this->draw_func_);
    this->opengl_initializer_.initialize(std::bind(&device::update, this));
  }
  ~device() {
    // TODO: implement
  }
  void
  set_screen_size(std::size_t screen_width,
                  std::size_t screen_height) {
    assert(0 < screen_width);
    assert(0 < screen_height);
    assert(screen_width  <= 4096);
    assert(screen_height <= 4096);
    this->screen_width_  = screen_width;
    this->screen_height_ = screen_height;
    this->graphics_context_.set_screen_size(screen_width, screen_height);
    this->to_destory_offscreen_texture_ = true;
  }
private:
  /*
   * NOTICE:
   *   The OpenGL functions should be called only in this method 'update'.
   *   Is that better to add an argument to this method?
   */
  bool
  update() {
    try {
      if (this->to_destory_offscreen_texture_) {
        if (this->offscreen_texture_) {
          // TODO: Should a texture include its framebuffer?
          this->graphics_context_.delete_framebuffer(*this->offscreen_texture_);
          this->offscreen_texture_ = nullptr;
        }
        this->to_destory_offscreen_texture_ = false;
      }
      if (!this->offscreen_texture_) {
        this->offscreen_texture_ = this->texture_factory_.create(this->screen_width_,
                                                                 this->screen_height_);
      }
      assert(static_cast<bool>(this->offscreen_texture_));
      bool const terminated = this->update_func_(this->texture_factory_);
      detail::graphics_context& g = this->graphics_context_;
      ::glEnable(GL_TEXTURE_2D);
      ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      g.set_offscreen(*this->offscreen_texture_);
      g.clear();
      this->draw_func_(g, *this->offscreen_texture_);
      g.flush();
      ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      g.reset_offscreen();
      g.clear();
      geometry_matrix geom_mat;
      geom_mat.set_a(this->screen_scale_);
      geom_mat.set_d(this->screen_scale_);
      g.draw_texture(*this->offscreen_texture_,
                     0, 0, this->screen_width_, this->screen_height_,
                     geom_mat, color_matrix::identity());
      g.flush();
      return terminated;
    } catch (std::runtime_error const& e) {
      // TODO: Logging
      std::cerr << e.what() << std::endl;
      std::abort();
    }
    return true;
  }
};

}
}
}

#endif
