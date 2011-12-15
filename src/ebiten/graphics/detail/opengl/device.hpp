#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_DEVICE_HPP
#define EBITEN_GRAPHICS_DETAIL_OPENGL_DEVICE_HPP

// TODO: refactoring
#ifndef EBITEN_IOS
# include "ebiten/frames/frame.hpp"
#endif

#include "ebiten/graphics/detail/opengl/graphics_context.hpp"
#include "ebiten/graphics/detail/opengl/opengl_initializer.hpp"
#include "ebiten/graphics/detail/opengl/texture_factory.hpp"
#include "ebiten/noncopyable.hpp"

#ifdef EBITEN_MACOSX
# include <OpenGL/gl.h>
#endif
#ifdef EBITEN_IOS
# import <GLKit/GLKit.h>
#endif

#include <cassert>
#include <functional>

// TODO: refactoring
#ifdef EBITEN_IOS
# define glOrtho glOrthof
#endif

namespace ebiten {
namespace graphics {
namespace detail {

class device : private noncopyable {
private:
  std::size_t const screen_width_;
  std::size_t const screen_height_;
  std::size_t const screen_scale_;
  std::function<void(device&)> update_func_;
  std::function<void(device&)> draw_func_;
  graphics_context graphics_context_;
  texture_factory texture_factory_;
  texture offscreen_texture_;
  GLuint offscreen_framebuffer_;
public:
#ifndef EBITEN_IOS
  device(std::size_t screen_width,
         std::size_t screen_height,
         std::size_t screen_scale,
         frames::frame& frame,
         std::function<void(device&)> const& update_func,
         std::function<void(device&)> const& draw_func)
    : screen_width_(screen_width),
      screen_height_(screen_height),
      screen_scale_(screen_scale),
      update_func_(update_func),
      draw_func_(draw_func) {
    assert(0 < this->screen_width_);
    assert(0 < this->screen_height_);
    assert(0 < this->screen_scale_);
    assert(this->update_func_);
    assert(this->draw_func_);
    opengl_initializer::initialize(frame, std::bind(&device::update, this));
  }
#endif
  template<class View>
  device(std::size_t screen_width,
         std::size_t screen_height,
         std::size_t screen_scale,
         View& view,
         std::function<void(device&)> const& update_func,
         std::function<void(device&)> const& draw_func)
    : screen_width_(screen_width),
      screen_height_(screen_height),
      screen_scale_(screen_scale),
      update_func_(update_func),
      draw_func_(draw_func) {
    assert(0 < this->screen_width_);
    assert(0 < this->screen_height_);
    assert(0 < this->screen_scale_);
    assert(this->update_func_);
    assert(this->draw_func_);
    opengl_initializer::initialize_with_view(view, std::bind(&device::update, this));
  }
  // TODO: destructor
  void
  update() {
    // TODO: If application is terminated, stop
    if (!this->offscreen_texture_) {
      this->initialize_offscreen();
    }
    this->update_func_(*this);
    ::glEnable(GL_TEXTURE_2D); // is not valid in OpenGL ES. Why?
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    {
      GLint origFramebuffer;
      ::glGetIntegerv(GL_FRAMEBUFFER_BINDING, &origFramebuffer);
      ::glBindFramebuffer(GL_FRAMEBUFFER, this->offscreen_framebuffer_);
      this->graphics_context_.clear();
      ::glEnable(GL_BLEND);
      ::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      std::size_t const width  = this->offscreen_texture_.width();
      std::size_t const height = this->offscreen_texture_.height();
      ::glViewport(0, 0,
                   static_cast<GLsizei>(width),
                   static_cast<GLsizei>(height));
      float const projection_matrix[] = {
        2.0 / width, 0,            0, 0,
        0,           2.0 / height, 0, 0,
        0,           0,            1, 0,
        -1,          -1,           0, 1,
      };
      this->graphics_context_.set_projection_matrix(std::begin(projection_matrix),
                                                    std::end(projection_matrix));
      this->graphics_context_.reset_geometry_matrix();
      this->graphics_context_.reset_color_matrix();
      this->draw_func_(*this);
      ::glFlush();
      ::glBindFramebuffer(GL_FRAMEBUFFER, origFramebuffer);
    }
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    this->graphics_context_.clear();
    ::glDisable(GL_BLEND);
    {
      std::size_t const width  = this->screen_width_  * this->screen_scale_;
      std::size_t const height = this->screen_height_ * this->screen_scale_;
      ::glViewport(0, 0,
                   static_cast<GLsizei>(width),
                   static_cast<GLsizei>(height));
      float const projection_matrix[] = {
        2.0 / width, 0,             0, 0,
        0,           -2.0 / height, 0, 0,
        0,           0,             1, 0,
        -1,          1,             0, 1,
      };
      this->graphics_context_.set_projection_matrix(std::begin(projection_matrix),
                                                    std::end(projection_matrix));
      geometry_matrix geometry_matrix(this->screen_scale_, 0,
                                      0, this->screen_scale_,
                                      0, 0);
      this->graphics_context_.set_geometry_matrix(geometry_matrix);
      this->graphics_context_.reset_color_matrix();
    }
    this->graphics_context_.set_texture(this->offscreen_texture_);
    this->graphics_context_.draw(0, 0, 0, 0, this->screen_width_, this->screen_height_);
    ::glFlush();
  }
  graphics_context&
  graphics_context() {
    return this->graphics_context_;
  }
  texture_factory&
  texture_factory() {
    return this->texture_factory_;
  }
private:
  void
  initialize_offscreen() {
    // TODO: need to lock context?
    this->offscreen_texture_ = texture_factory().create(this->screen_width_,
                                                        this->screen_height_);
    ::glGenFramebuffers(1, &this->offscreen_framebuffer_);
    {
      GLint origFramebuffer;
      ::glGetIntegerv(GL_FRAMEBUFFER_BINDING, &origFramebuffer);
      ::glBindFramebuffer(GL_FRAMEBUFFER, this->offscreen_framebuffer_);
      ::glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D,
                               this->offscreen_texture_.id(),
                               0);
      ::glBindFramebuffer(GL_FRAMEBUFFER, origFramebuffer);
    }
    if (::glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
      throw std::runtime_error("framebuffer is not supported completely");
    }
  }
};

}
}
}

#endif
