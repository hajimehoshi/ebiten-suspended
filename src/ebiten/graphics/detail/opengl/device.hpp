#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_DEVICE_HPP
#define EBITEN_GRAPHICS_DETAIL_OPENGL_DEVICE_HPP

#include "ebiten/frames/frame.hpp"
#include "ebiten/graphics/detail/opengl/graphics_context.hpp"
#include "ebiten/graphics/detail/opengl/initialize_opengl.hpp"
#include "ebiten/graphics/detail/opengl/texture_factory.hpp"
#include "ebiten/graphics/sprite.hpp"
#include "ebiten/noncopyable.hpp"
#include <OpenGL/gl.h>
#include <cassert>
#include <functional>

namespace ebiten {
namespace graphics {
namespace detail {

class device : private noncopyable {
public:
  typedef frames::frame frame_type;
  typedef detail::texture_factory texture_factory_type;
  typedef detail::graphics_context graphics_context_type;
private:
  std::size_t const screen_width_;
  std::size_t const screen_height_;
  std::size_t const screen_scale_;
  std::function<void(device&)> draw_func_;
  graphics_context_type graphics_context_;
  texture_factory_type texture_factory_;
  std::unique_ptr<texture const> offscreen_texture_;
  GLuint framebuffer_;
public:
  device(std::size_t screen_width,
         std::size_t screen_height,
         std::size_t screen_scale,
         frames::frame& frame,
         std::function<void(device&)> const& draw_func)
    : screen_width_(screen_width),
      screen_height_(screen_height),
      screen_scale_(screen_scale),
      draw_func_(draw_func),
      graphics_context_(),
      texture_factory_() {
    initialize_opengl(frame, std::bind(&device::update, this));
    this->offscreen_texture_ = texture_factory().create(screen_width, screen_height);
    ::glGenFramebuffersEXT(1, &this->framebuffer_);
    assert(this->framebuffer_);
    ::glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, this->framebuffer_);
    ::glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
                                GL_COLOR_ATTACHMENT0_EXT,
                                GL_TEXTURE_2D,
                                this->offscreen_texture_->id(),
                                0);
    if (::glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT) {
      throw std::runtime_error("framebuffer is not supported completely");
    }
    ::glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  }
  void
  update() {
    float const offscreen_width  = static_cast<float>(this->offscreen_texture_->width());
    float const offscreen_height = static_cast<float>(this->offscreen_texture_->height());
    float const offscreen_tu     = offscreen_width  / this->offscreen_texture_->texture_width();
    float const offscreen_tv     = offscreen_height / this->offscreen_texture_->texture_height();
    float const offscreen_vertex[4][3] = {{0,               0,                0},
                                          {offscreen_width, 0,                0},
                                          {offscreen_width, offscreen_height, 0},
                                          {0,               offscreen_height, 0}};
    float const screen_scale_f = static_cast<float>(this->screen_scale_);
    float const offscreen_geo[] = {screen_scale_f, 0,              0, 0,
                                   0,              screen_scale_f, 0, 0,
                                   0,              0,              1, 0,
                                   0,              0,              0, 1};
    ::glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, this->framebuffer_);
    ::glClearColor(0, 0, 0, 1);
    ::glClear(GL_COLOR_BUFFER_BIT);
    ::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    ::glEnable(GL_TEXTURE_2D);
    ::glEnable(GL_BLEND);
    ::glViewport(0, 0, this->screen_width_, this->screen_height_);
    ::glMatrixMode(GL_PROJECTION);
    ::glLoadIdentity();
    ::glOrtho(0, this->screen_width_, 0, this->screen_height_, 0, 1);
    ::glMatrixMode(GL_MODELVIEW);
    ::glLoadIdentity();
    ::glUseProgram(0);
    this->draw_func_(*this);
    ::glFlush();
    ::glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    // render the offscreen to the screen
    ::glClearColor(0, 0, 0, 1);
    ::glClear(GL_COLOR_BUFFER_BIT);
    ::glEnable(GL_TEXTURE_2D);
    ::glDisable(GL_BLEND);
    ::glViewport(0, 0,
                 this->screen_width_ * this->screen_scale_,
                 this->screen_height_ * this->screen_scale_);
    ::glMatrixMode(GL_PROJECTION);
    ::glLoadIdentity();
    ::glOrtho(0, this->screen_width_ * this->screen_scale_,
              this->screen_height_ * this->screen_scale_, 0,
              0, 1);
    ::glMatrixMode(GL_MODELVIEW);
    ::glLoadMatrixf(offscreen_geo);
    ::glBindTexture(GL_TEXTURE_2D, this->offscreen_texture_->id());
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    {
      ::glBegin(GL_QUADS);
      ::glTexCoord2f(0, 0);
      ::glVertex3fv(offscreen_vertex[0]);
      ::glTexCoord2f(offscreen_tu, 0);
      ::glVertex3fv(offscreen_vertex[1]);
      ::glTexCoord2f(offscreen_tu, offscreen_tv);
      ::glVertex3fv(offscreen_vertex[2]);
      ::glTexCoord2f(0, offscreen_tv);
      ::glVertex3fv(offscreen_vertex[3]);
      ::glEnd();
    }
    ::glBindTexture(GL_TEXTURE_2D, 0);
    ::glFlush();
  }
  graphics_context_type&
  graphics_context() {
    return this->graphics_context_;
  }
  texture_factory_type&
  texture_factory() {
    return this->texture_factory_;
  }
};

}
}
}

#endif
