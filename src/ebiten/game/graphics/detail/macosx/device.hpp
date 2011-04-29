#ifndef EBITEN_GAME_GRAPHICS_DETAIL_MACOSX_DEVICE_HPP
#define EBITEN_GAME_GRAPHICS_DETAIL_MACOSX_DEVICE_HPP

#include "ebiten/game/graphics/detail/macosx/graphics_context.hpp"
#include "ebiten/game/graphics/detail/macosx/texture_factory.hpp"
#include "ebiten/game/graphics/detail/macosx/texture_id.hpp"
#include "ebiten/game/graphics/detail/macosx/view.hpp"
#include "ebiten/game/graphics/sprite.hpp"
#include <OpenGL/gl.h>
#include <boost/function.hpp>
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
  typedef typename detail::view<Frame> view_type;
  std::size_t const screen_width_;
  std::size_t const screen_height_;
  std::size_t const window_scale_;
  boost::function<void()> draw_sprites_;
  view_type view_;
  texture_factory_type texture_factory_;
  graphics_context_type graphics_context_;
  texture const offscreen_texture_;
  GLuint const framebuffer_;
public:
  device(std::size_t screen_width,
         std::size_t screen_height,
         std::size_t window_scale,
         boost::function<void()>& draw_sprites)
    : screen_width_(screen_width),
      screen_height_(screen_height),
      window_scale_(window_scale),
      draw_sprites_(draw_sprites),
      view_(screen_width * window_scale,
            screen_height * window_scale,
            boost::bind(&device<Frame>::update, this)),
      texture_factory_(),
      graphics_context_(),
      offscreen_texture_(texture_factory().create(screen_width, screen_height)),
      framebuffer_(generate_frame_buffer()) {
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
    this->draw_sprites_();
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
    return this->view_.frame();
  }
  graphics_context_type&
  graphics_context() {
    return this->graphics_context_;
  }
  texture_factory_type&
  texture_factory() {
    return this->texture_factory_;
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
