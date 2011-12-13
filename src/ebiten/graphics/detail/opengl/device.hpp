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
    assert(::glGetError() == GL_NO_ERROR);
    {
      GLint origFramebuffer;
      ::glGetIntegerv(GL_FRAMEBUFFER_BINDING, &origFramebuffer);
      assert(::glGetError() == GL_NO_ERROR);
      ::glBindFramebuffer(GL_FRAMEBUFFER, this->offscreen_framebuffer_);
      assert(::glGetError() == GL_NO_ERROR);
      ::glClearColor(0, 0, 0, 1);
      assert(::glGetError() == GL_NO_ERROR);
      ::glClear(GL_COLOR_BUFFER_BIT);
      assert(::glGetError() == GL_NO_ERROR);
      // ::glEnable(GL_TEXTURE_2D);  is not valid in OpenGL ES. Why?
      ::glEnable(GL_BLEND);
      assert(::glGetError() == GL_NO_ERROR);
      ::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      assert(::glGetError() == GL_NO_ERROR);
      ::glViewport(0, 0,
                   static_cast<GLsizei>(this->screen_width_),
                   static_cast<GLsizei>(this->screen_height_));
      assert(::glGetError() == GL_NO_ERROR);
      ::glMatrixMode(GL_PROJECTION);
      assert(::glGetError() == GL_NO_ERROR);
      ::glLoadIdentity();
      assert(::glGetError() == GL_NO_ERROR);
      ::glOrtho(0, this->screen_width_, 0, this->screen_height_, 0, 1);
      assert(::glGetError() == GL_NO_ERROR);
      ::glMatrixMode(GL_MODELVIEW);
      assert(::glGetError() == GL_NO_ERROR);
      ::glLoadIdentity();
      assert(::glGetError() == GL_NO_ERROR);
      ::glUseProgram(0);
      assert(::glGetError() == GL_NO_ERROR);
      this->draw_func_(*this);
      ::glFlush();
      assert(::glGetError() == GL_NO_ERROR);
      ::glBindFramebuffer(GL_FRAMEBUFFER, origFramebuffer);
      assert(::glGetError() == GL_NO_ERROR);
    }

    // render the offscreen to the screen
    ::glClearColor(0, 0, 0, 1);
    ::glClear(GL_COLOR_BUFFER_BIT);
    ::glEnable(GL_TEXTURE_2D);
    ::glDisable(GL_BLEND);
    ::glViewport(0, 0,
                 static_cast<GLsizei>(this->screen_width_  * this->screen_scale_),
                 static_cast<GLsizei>(this->screen_height_ * this->screen_scale_));
    ::glMatrixMode(GL_PROJECTION);
    ::glLoadIdentity();
    ::glOrtho(0, this->screen_width_ * this->screen_scale_,
              this->screen_height_ * this->screen_scale_, 0,
              0, 1);
    assert(::glGetError() == GL_NO_ERROR);
    ::glMatrixMode(GL_MODELVIEW);
    {
      float const screen_scale_f = static_cast<float>(this->screen_scale_);
      float const offscreen_geo[] = {screen_scale_f, 0,              0, 0,
                                     0,              screen_scale_f, 0, 0,
                                     0,              0,              1, 0,
                                     0,              0,              0, 1};
      ::glLoadMatrixf(offscreen_geo);
    }
    ::glBindTexture(GL_TEXTURE_2D, this->offscreen_texture_.id());
    assert(::glGetError() == GL_NO_ERROR);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    {
      float const offscreen_width  = static_cast<float>(this->screen_width_);
      float const offscreen_height = static_cast<float>(this->screen_height_);
      float const vertex_pointer[] = {0,               0,
                                      offscreen_width, 0,
                                      0,               offscreen_height,
                                      offscreen_width, offscreen_height,};
      float const offscreen_tu = offscreen_width  / this->offscreen_texture_.width();
      float const offscreen_tv = offscreen_height / this->offscreen_texture_.height();
      float tex_coord[] = {0,            0,
                           offscreen_tu, 0,
                           0,            offscreen_tv,
                           offscreen_tu, offscreen_tv,};
      ::glEnableClientState(GL_VERTEX_ARRAY);
      ::glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      ::glVertexPointer(2, GL_FLOAT, 0, vertex_pointer);
      ::glTexCoordPointer(2, GL_FLOAT, 0, tex_coord);
      ::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
      ::glDisableClientState(GL_TEXTURE_COORD_ARRAY);
      ::glDisableClientState(GL_VERTEX_ARRAY);
    }
    ::glBindTexture(GL_TEXTURE_2D, 0);
    assert(::glGetError() == GL_NO_ERROR);
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
    assert(this->offscreen_framebuffer_);
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
