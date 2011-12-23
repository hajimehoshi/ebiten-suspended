#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_GRAPHICS_CONTEXT_HPP
#define EBITEN_GRAPHICS_DETAIL_OPENGL_GRAPHICS_CONTEXT_HPP

#include "ebiten/graphics/detail/opengl/texture_factory.hpp"
#include "ebiten/graphics/color_matrix.hpp"
#include "ebiten/graphics/geometry_matrix.hpp"
#include "ebiten/graphics/texture.hpp"
#include "ebiten/noncopyable.hpp"
#include "ebiten/platform.hpp"

#ifdef EBITEN_MACOSX
# include <OpenGL/gl.h>
#endif
#ifdef EBITEN_IOS
# import <GLKit/GLKit.h>
#endif

#include <array>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <regex>
#include <sstream>
#include <unordered_map>

namespace ebiten {
namespace graphics {
namespace detail {

class device;

class graphics_context : private noncopyable {
  friend class device;
private:
  texture_factory& texture_factory_;
  GLuint regular_shader_program_;
  GLuint color_mat_shader_program_;
  GLuint current_program_;
  std::array<float, 16> projection_matrix_;
  texture empty_texture_;
  std::unordered_map<texture_id, GLuint> framebuffers_;
  bool main_framebuffer_initialized_;
  GLuint main_framebuffer_;
private:
  graphics_context(texture_factory& texture_factory)
    : texture_factory_(texture_factory),
      regular_shader_program_(0),
      color_mat_shader_program_(0),
      current_program_(0),
      main_framebuffer_initialized_(false),
      main_framebuffer_(0) {
  }
public:
  void
  clear() {
    ::glClearColor(0, 0, 0, 1);
    ::glClear(GL_COLOR_BUFFER_BIT);
  }
  // Only for debugging?
  void
  draw_rect(std::size_t x, std::size_t y, std::size_t width, std::size_t height,
            uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
    if (!this->empty_texture_) {
      this->empty_texture_ = this->texture_factory_.create(16, 16);
    }

    geometry_matrix geo_mat;
    geo_mat.set_a(width  / this->empty_texture_.width());
    geo_mat.set_d(height / this->empty_texture_.height());
    geo_mat.set_tx(x);
    geo_mat.set_ty(y);

    color_matrix color_mat;
    color_mat.set_element<0, 4>(red   / 255.0);
    color_mat.set_element<1, 4>(green / 255.0);
    color_mat.set_element<2, 4>(blue  / 255.0);
    color_mat.set_element<3, 4>(alpha / 255.0);

    this->draw_texture(this->empty_texture_,
                       0, 0, this->empty_texture_.width(), this->empty_texture_.height(),
                       geo_mat, color_mat);
  }
  // TODO: dst_width / dst_height?
  void
  draw_texture(texture const& texture,
               double src_x, double src_y, double width, double height,
               geometry_matrix const& geometry_matrix,
               color_matrix const& color_matrix) {
    // TODO: Throwing an exception?
    if (!texture) {
      return;
    }
    this->set_shader_program(geometry_matrix, color_matrix);
    // TODO: cache? Check other callings of glBindTexture.
    ::glBindTexture(GL_TEXTURE_2D, texture.id());
    // TODO: replace float to short?
    // http://objective-audio.jp/2009/07/ngmoco-opengl.html
    // 選べるようにするといいかも
    float const texture_width  = texture.texture_width();
    float const texture_height = texture.texture_height();
    float const tu1 = src_x            / texture_width;
    float const tu2 = (src_x + width)  / texture_width;
    float const tv1 = src_y            / texture_height;
    float const tv2 = (src_y + height) / texture_height;
    float const x1 = 0;
    float const x2 = width;
    float const y1 = 0;
    float const y2 = height;
    float const vertex[] = {x1, y1,
                            x2, y1,
                            x1, y2,
                            x2, y2,};
    float const tex_coord[] = {tu1, tv1,
                               tu2, tv1,
                               tu1, tv2,
                               tu2, tv2,};

    GLint const vertex_attr_location = ::glGetAttribLocation(this->current_program_,
                                                             "vertex");
    assert(vertex_attr_location != -1);
    GLint const texture_attr_location = ::glGetAttribLocation(this->current_program_,
                                                              "texture");
    assert(texture_attr_location != -1);

    // TODO: use glMultiDrawArrays?
    ::glEnableClientState(GL_VERTEX_ARRAY);
    ::glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    ::glEnableVertexAttribArray(vertex_attr_location);
    ::glEnableVertexAttribArray(texture_attr_location);
    ::glVertexAttribPointer(vertex_attr_location, 2, GL_FLOAT, GL_FALSE,
                            0, vertex);
    ::glVertexAttribPointer(texture_attr_location, 2, GL_FLOAT, GL_FALSE,
                            0, tex_coord);
    ::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    ::glDisableVertexAttribArray(texture_attr_location);
    ::glDisableVertexAttribArray(vertex_attr_location);
    ::glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    ::glDisableClientState(GL_VERTEX_ARRAY);
  }
  // TODO: arguments seem to be strange
  void
  set_offscreen(texture const& texture,
                float left, float right, float bottom, float top) {
    if (!this->main_framebuffer_initialized_) {
      GLint framebuffer;
      ::glGetIntegerv(GL_FRAMEBUFFER_BINDING, &framebuffer);
      this->main_framebuffer_ = framebuffer;
      this->main_framebuffer_initialized_ = true;
    }
    GLuint framebuffer = this->get_framebuffer(texture);
    ::glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    this->clear();
    ::glEnable(GL_BLEND);
    ::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    float const width  = right - left;
    float const height = top - bottom;
    float const tx     = - (right + left) / (right - left);
    float const ty     = - (top + bottom) / (top - bottom);
    ::glViewport(std::min(left, right),
                 std::min(bottom, top),
                 static_cast<GLsizei>(std::abs(width)),
                 static_cast<GLsizei>(std::abs(height)));
    float const projection_matrix[] = {
      2.0 / width, 0,            0, 0,
      0,           2.0 / height, 0, 0,
      0,           0,            1, 0,
      tx,          ty,           0, 1,
    };
    std::copy(std::begin(projection_matrix),
              std::end(projection_matrix),
              this->projection_matrix_.data());
  }
  void
  reset_offscreen(float left, float right, float bottom, float top) {
    this->set_offscreen(graphics::texture(),
                        left, right, bottom, top);
  }
  void
  flush() {
    ::glFlush();
  }
private:
  void
  set_shader_program(geometry_matrix const& geometry_matrix,
                     color_matrix const& color_matrix) {
    if (!(this->regular_shader_program_ &&
          this->color_mat_shader_program_)) {
      // TODO: Replace that with logging
      try {
        this->initialize_shader_programs();
      } catch (std::runtime_error const& err) {
        std::cerr << err.what() << std::endl;
        throw;
      }
    }
    GLuint program;
    if (color_matrix.is_identity()) {
      program = this->regular_shader_program_;
    } else {
      program = this->color_mat_shader_program_;
    }
    // TODO: cache and skip?
    ::glUseProgram(program);
    this->current_program_ = program;
    {
      GLint location = ::glGetUniformLocation(program, "projection_matrix");
      assert(location != -1);
      ::glUniformMatrix4fv(location,
                           1, GL_FALSE, this->projection_matrix_.data());
    }
    {
      graphics::geometry_matrix const& mat = geometry_matrix;
      float const gl_modelview_mat[] = {mat.a(),  mat.c(),  0, 0,
                                        mat.b(),  mat.d(),  0, 0,
                                        0,        0,        1, 0,
                                        mat.tx(), mat.ty(), 0, 1};
      {
        GLint location = ::glGetUniformLocation(program, "modelview_matrix");
        assert(location != -1);
        ::glUniformMatrix4fv(location,
                             1, GL_FALSE, gl_modelview_mat);
      }
    }
    {
      GLint location = ::glGetUniformLocation(program, "texture");
      assert(location != -1);
      ::glUniform1i(location, 0);
    }
    if (program == this->color_mat_shader_program_) {
      graphics::color_matrix const& mat = color_matrix;
      // TODO: Refactoring
      float const gl_color_mat[] = {
        static_cast<float>(mat.element<0, 0>()),
        static_cast<float>(mat.element<0, 1>()),
        static_cast<float>(mat.element<0, 2>()),
        static_cast<float>(mat.element<0, 3>()),
        static_cast<float>(mat.element<1, 0>()),
        static_cast<float>(mat.element<1, 1>()),
        static_cast<float>(mat.element<1, 2>()),
        static_cast<float>(mat.element<1, 3>()),
        static_cast<float>(mat.element<2, 0>()),
        static_cast<float>(mat.element<2, 1>()),
        static_cast<float>(mat.element<2, 2>()),
        static_cast<float>(mat.element<2, 3>()),
        static_cast<float>(mat.element<3, 0>()),
        static_cast<float>(mat.element<3, 1>()),
        static_cast<float>(mat.element<3, 2>()),
        static_cast<float>(mat.element<3, 3>()),
      };
      {
        GLint location = glGetUniformLocation(program, "color_matrix");
        assert(location != -1);
        ::glUniformMatrix4fv(location, 1, GL_FALSE, gl_color_mat);
      }
      float const gl_color_mat_translation[] = {
        static_cast<float>(mat.element<0, 4>()),
        static_cast<float>(mat.element<1, 4>()),
        static_cast<float>(mat.element<2, 4>()),
        static_cast<float>(mat.element<3, 4>()),
      };
      {
        GLint location = glGetUniformLocation(program, "color_matrix_translation");
        assert(location != -1);
        ::glUniform4fv(location, 1, gl_color_mat_translation);
      }
    }
  }
  GLuint
  get_framebuffer(texture const& texture) {
    if (!texture) {
      return this->main_framebuffer_;      
    }
    auto const& it = this->framebuffers_.find(texture.id());
    if (it != this->framebuffers_.end()) {
      return it->second;
    }
    GLuint framebuffer;
    ::glGenFramebuffers(1, &framebuffer);
    {
      GLint orig_framebuffer;
      ::glGetIntegerv(GL_FRAMEBUFFER_BINDING, &orig_framebuffer);
      ::glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
      ::glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D,
                               texture.id(),
                               0);
      ::glBindFramebuffer(GL_FRAMEBUFFER, orig_framebuffer);
    }
    if (::glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
      throw std::runtime_error("framebuffer is not supported completely");
    }
    return this->framebuffers_[texture.id()] = framebuffer;
  }
  // TODO: Check multi process
  bool
  is_opengl_es_2() {
    GLubyte const* gl_version_p = glGetString(GL_VERSION);
    std::string const gl_version(reinterpret_cast<char const*>(gl_version_p));
    std::smatch match;
    std::regex const reg("^OpenGL ES ([[:digit:]])");
    bool const matched = std::regex_search(gl_version, match, reg);
    return matched && (match[1] == '2');
  }
  void
  initialize_shader_programs() {
    std::string vertex_shader_src;
    std::string fragment_shader_src;
    std::string color_mat_shader_src;
    vertex_shader_src +=
      "attribute highp vec2 vertex;\n"
      "attribute highp vec2 texture;\n"
      "uniform highp mat4 projection_matrix;\n"
      "uniform highp mat4 modelview_matrix;\n"
      "varying highp vec2 tex_coord;\n"
      "\n"
      "void main(void) {\n"
      "  tex_coord = texture;\n"
      "  gl_Position = projection_matrix * modelview_matrix * vec4(vertex, 0, 1);\n"
      "}\n";
    fragment_shader_src +=
      "uniform lowp sampler2D texture;\n"
      "varying highp vec2 tex_coord;\n"
      "\n"
      "void main(void) {\n"
      "  gl_FragColor = texture2D(texture, tex_coord);\n"
      "}\n";
    color_mat_shader_src +=
      "uniform highp sampler2D texture;\n"
      "uniform lowp mat4 color_matrix;\n"
      "uniform lowp vec4 color_matrix_translation;\n"
      "varying highp vec2 tex_coord;\n"
      "\n"
      "void main(void) {\n"
      "  lowp vec4 color = texture2D(texture, tex_coord);\n"
      "  gl_FragColor = (color_matrix * color) + color_matrix_translation;\n"
      "}\n";
    if (!is_opengl_es_2()) {
      std::regex const reg("(highp|lowp)");
      vertex_shader_src    = std::regex_replace(vertex_shader_src,    reg, "");
      fragment_shader_src  = std::regex_replace(fragment_shader_src,  reg, "");
      color_mat_shader_src = std::regex_replace(color_mat_shader_src, reg, "");
    }
    // TODO: ARB?
    GLuint vertex_shader = ::glCreateShader(GL_VERTEX_SHADER);
    assert(vertex_shader);
    GLuint fragment_shader = ::glCreateShader(GL_FRAGMENT_SHADER);
    assert(fragment_shader);
    GLuint color_mat_shader = ::glCreateShader(GL_FRAGMENT_SHADER);
    assert(color_mat_shader);
    this->compile_shader(vertex_shader,    vertex_shader_src,    "vertex_shader");
    this->compile_shader(fragment_shader,  fragment_shader_src,  "fragment_shader");
    this->compile_shader(color_mat_shader, color_mat_shader_src, "color_mat_shader");
    // TODO: Refactoring
    {
      GLuint program = ::glCreateProgram();
      assert(program);
      ::glAttachShader(program, vertex_shader);
      ::glAttachShader(program, fragment_shader);
      ::glLinkProgram(program);
      GLint linked;
      ::glGetProgramiv(program, GL_LINK_STATUS, &linked);
      if (linked == GL_FALSE) {
        throw std::runtime_error("program error");
      }
      this->regular_shader_program_ = program;
    }
    {
      GLuint program = ::glCreateProgram();
      assert(program);
      ::glAttachShader(program, vertex_shader);
      ::glAttachShader(program, color_mat_shader);
      ::glLinkProgram(program);
      GLint linked;
      ::glGetProgramiv(program, GL_LINK_STATUS, &linked);
      if (linked == GL_FALSE) {
        throw std::runtime_error("program error");
      }
      this->color_mat_shader_program_ = program;
    }
    ::glDeleteShader(vertex_shader);
    ::glDeleteShader(fragment_shader);
    ::glDeleteShader(color_mat_shader);
  }
  void
  compile_shader(GLuint shader, std::string const& src, std::string const& shader_name) {
    char const* src_p = src.c_str();
    ::glShaderSource(shader, 1, &src_p, 0);
    ::glCompileShader(shader);
    GLint compiled;
    ::glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    this->show_shader_log(shader, shader_name);
    if (compiled == GL_FALSE) {
      throw std::runtime_error("shader compile error (" + shader_name + ")");
    }
  }
  // TODO: Debug Mode
  void
  show_shader_log(GLuint shader, std::string const& shader_name) {
    int log_size = 0;
    ::glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_size);
    if (log_size) {
      int length;
      std::unique_ptr<char[]> buffer(new char[log_size]);
      std::fill_n(buffer.get(), log_size, 0);
      ::glGetShaderInfoLog(shader,
                           log_size,
                           &length,
                           buffer.get());
      std::ostringstream msg;
      msg << "shader error (" << shader_name << "): "
          << buffer.get();
      throw std::runtime_error(msg.str());
    }
  }
  // TODO: show_program_log
};

}
}
}

#endif
