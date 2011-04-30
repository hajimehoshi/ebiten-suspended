#ifndef EBITEN_GAME_GRAPHICS_DETAIL_OPENGL_DEVICE_HPP
#define EBITEN_GAME_GRAPHICS_DETAIL_OPENGL_DEVICE_HPP

#include "ebiten/game/graphics/detail/opengl/graphics_context.hpp"
#include "ebiten/game/graphics/detail/opengl/texture_factory.hpp"
#include "ebiten/game/graphics/sprite.hpp"
#include <OpenGL/gl.h>
#include <boost/signals2/signal.hpp>
#include <cassert>

namespace ebiten {
namespace game {
namespace graphics {
namespace detail {

template<class Frame>
class device : private boost::noncopyable {
public:
  typedef Frame frame_type;
  typedef typename detail::texture_factory texture_factory_type;
  typedef typename detail::graphics_context graphics_context_type;
private:
  std::size_t const screen_width_;
  std::size_t const screen_height_;
  std::size_t const window_scale_;
  Frame frame_;
  texture_factory_type texture_factory_;
  graphics_context_type graphics_context_;
  texture const offscreen_texture_;
  GLuint const framebuffer_;
  boost::signals2::signal<void()> drawing_sprites_;
public:
  device(std::size_t screen_width,
         std::size_t screen_height,
         std::size_t window_scale)
    : screen_width_(screen_width),
      screen_height_(screen_height),
      window_scale_(window_scale),
      frame_(screen_width * window_scale, screen_height * window_scale),
      texture_factory_(),
      graphics_context_(),
      offscreen_texture_(texture_factory().create(screen_width, screen_height)),
      framebuffer_(generate_frame_buffer()) {
    this->frame_.connect_updating(boost::bind(&device<Frame>::update, this));
    ::glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, this->framebuffer_);
    ::glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
                                GL_COLOR_ATTACHMENT0_EXT,
                                GL_TEXTURE_2D,
                                this->offscreen_texture_.id(),
                                0);
    if (::glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT) {
      throw std::runtime_error("framebuffer is not supported completely");
    }
    ::glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  }
  void
  update() {
    float const offscreen_width  = static_cast<float>(this->offscreen_texture_.width());
    float const offscreen_height = static_cast<float>(this->offscreen_texture_.height());
    float const offscreen_tu     = offscreen_width  / this->offscreen_texture_.texture_width();
    float const offscreen_tv     = offscreen_height / this->offscreen_texture_.texture_height();
    float const offscreen_vertex[4][3] = {{0,               0,                0},
                                          {offscreen_width, 0,                0},
                                          {offscreen_width, offscreen_height, 0},
                                          {0,               offscreen_height, 0}};
    float const window_scale_f = static_cast<float>(this->window_scale_);
    float const offscreen_geo[] = {window_scale_f, 0,              0, 0,
                                   0,              window_scale_f, 0, 0,
                                   0,              0,              1, 0,
                                   0,              0,              0, 1};
    // render sprites to the offscreen
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
    this->drawing_sprites_();
    ::glFlush();
    ::glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    // render the offscreen to the screen
    ::glClearColor(0, 0, 0, 1);
    ::glClear(GL_COLOR_BUFFER_BIT);
    ::glEnable(GL_TEXTURE_2D);
    ::glDisable(GL_BLEND);
    ::glViewport(0, 0,
                 this->screen_width_ * this->window_scale_,
                 this->screen_height_ * this->window_scale_);
    ::glMatrixMode(GL_PROJECTION);
    ::glLoadIdentity();
    ::glOrtho(0, this->screen_width_ * this->window_scale_,
              this->screen_height_ * this->window_scale_, 0,
              0, 1);
    ::glMatrixMode(GL_MODELVIEW);
    ::glLoadMatrixf(offscreen_geo);
    ::glBindTexture(GL_TEXTURE_2D, this->offscreen_texture_.id());
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
  frame_type&
  frame() {
    return this->frame_;
  }
  graphics_context_type&
  graphics_context() {
    return this->graphics_context_;
  }
  texture_factory_type&
  texture_factory() {
    return this->texture_factory_;
  }
  template<class Func>
  void
  connect_drawing_sprites(Func const& func) {
    this->drawing_sprites_.connect(func);
  }
private:
  static GLuint
  generate_frame_buffer() {
    GLuint framebuffer = 0;
    ::glGenFramebuffersEXT(1, &framebuffer);
    assert(framebuffer);
    return framebuffer;
  }
};

}
}
}
}

#endif
