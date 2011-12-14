#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_GRAPHICS_CONTEXT_HPP
#define EBITEN_GRAPHICS_DETAIL_OPENGL_GRAPHICS_CONTEXT_HPP

#include "ebiten/graphics/detail/opengl/device.hpp"
#include "ebiten/graphics/color_matrix.hpp"
#include "ebiten/graphics/drawing_region.hpp"
#include "ebiten/graphics/geometry_matrix.hpp"
#include "ebiten/graphics/texture.hpp"
#include "ebiten/noncopyable.hpp"

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

namespace ebiten {
namespace graphics {
namespace detail {

class graphics_context : private noncopyable {
  friend class device;
private:
  GLuint vertex_shader_program_;
  GLuint vertex_fragment_shader_program_;
  texture current_texture_;
  std::array<float, 16> projection_matrix_;
  geometry_matrix modelview_matrix_;
  color_matrix color_matrix_;
private:
  graphics_context()
    : vertex_shader_program_(0),
      vertex_fragment_shader_program_(0) {
    this->modelview_matrix_.set_identity();
    this->color_matrix_.set_identity();
    assert(this->modelview_matrix_.is_identity());
  }
public:
  // Only for debugging?
  void
  draw_rect(std::size_t x, std::size_t y, std::size_t width, std::size_t height,
            uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
    this->set_shader_program();
    float const vertex[] = {x,         y,
                            x + width, y,
                            x,         y + height,
                            x + width, y + height,};
    /*uint8_t const colors[] = {red, green, blue, alpha,
                              red, green, blue, alpha,
                              red, green, blue, alpha,
                              red, green, blue, alpha,};*/
    ::glEnableClientState(GL_VERTEX_ARRAY);
    //::glEnableClientState(GL_COLOR_ARRAY);
    ::glColor4ub(red, green, blue, alpha);
    ::glVertexPointer(2, GL_FLOAT, 0, vertex);
    //::glColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
    assert(::glGetError() == GL_NO_ERROR);
    ::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    ::glDisableClientState(GL_VERTEX_ARRAY);
    //::glDisableClientState(GL_COLOR_ARRAY);
    ::glColor4f(1, 1, 1, 1);
  }
  void
  set_texture(texture const& texture) {
    this->current_texture_ = texture;
  }
  void
  reset_texture() {
    this->current_texture_ = ebiten::graphics::texture();
  }
  void
  draw(drawing_region const& dr) {
    // TODO: Throwing an exception?
    if (!this->current_texture_) {
      return;
    }
    this->set_shader_program();
    ::glBindTexture(GL_TEXTURE_2D, this->current_texture_.id());
    assert(::glGetError() == GL_NO_ERROR);
    // TODO: replace float to short?
    // http://objective-audio.jp/2009/07/ngmoco-opengl.html
    // 選べるようにするといいかも
    float const texture_width  = this->current_texture_.width();
    float const texture_height = this->current_texture_.height();
    float const tu1 = dr.src_x                 / texture_width;
    float const tu2 = (dr.src_x + dr.width)  / texture_width;
    float const tv1 = dr.src_y                 / texture_height;
    float const tv2 = (dr.src_y + dr.height) / texture_height;
    float const x1 = dr.dst_x;
    float const x2 = dr.dst_x + dr.width;
    float const y1 = dr.dst_y;
    float const y2 = dr.dst_y + dr.height;
    float const vertex[] = {x1, y1,
                            x2, y1,
                            x1, y2,
                            x2, y2,};
    float const tex_coord[] = {tu1, tv1,
                               tu2, tv1,
                               tu1, tv2,
                               tu2, tv2,};
    // TODO: use glMultiDrawArrays?
    ::glEnableClientState(GL_VERTEX_ARRAY);
    ::glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    ::glVertexPointer(2, GL_FLOAT, 0, vertex);
    ::glTexCoordPointer(2, GL_FLOAT, 0, tex_coord);
    ::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    ::glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    ::glDisableClientState(GL_VERTEX_ARRAY);
  }
  void
  set_geometry_matrix(geometry_matrix const& mat) {
    this->modelview_matrix_ = mat;
  }
  void
  reset_geometry_matrix() {
    this->modelview_matrix_.set_identity();
  }
  void
  set_color_matrix(color_matrix const& mat) {
    this->color_matrix_ = mat;
  }
  void
  reset_color_matrix() {
    this->color_matrix_.set_identity();
  }
private:
  void
  set_shader_program() {
    if (!this->vertex_shader_program_ ||
        !this->vertex_fragment_shader_program_) {
      // TODO: Replace that with logging
      try {
        this->initialize_shader_programs();
      } catch (std::runtime_error const& err) {
        std::cerr << err.what() << std::endl;
        throw;
      }
    }
    GLuint program;
    if (this->color_matrix_.is_identity()) {
      program = this->vertex_shader_program_;
    } else {
      program = this->vertex_fragment_shader_program_;
    }
    // TODO: cache and skip?
    ::glUseProgram(program);
    assert(::glGetError() == GL_NO_ERROR);
    {
      GLint location = ::glGetUniformLocation(program, "projection_matrix");
      assert(::glGetError() == GL_NO_ERROR);
      assert(location != -1);
      ::glUniformMatrix4fv(location,
                           1, GL_FALSE, this->projection_matrix_.data());
      assert(::glGetError() == GL_NO_ERROR);
    }
    {
      geometry_matrix mat = this->modelview_matrix_;
      float const gl_modelview_mat[] = {mat.a(),  mat.c(),  0, 0,
                                        mat.b(),  mat.d(),  0, 0,
                                        0,        0,        1, 0,
                                        mat.tx(), mat.ty(), 0, 1};
      {
        GLint location = ::glGetUniformLocation(program, "modelview_matrix");
        assert(::glGetError() == GL_NO_ERROR);
        assert(location != -1);
        ::glUniformMatrix4fv(location,
                             1, GL_FALSE, gl_modelview_mat);
        assert(::glGetError() == GL_NO_ERROR);
      }
    }
    {
      GLint location = ::glGetUniformLocation(program, "texture");
      assert(::glGetError() == GL_NO_ERROR);
      //assert(location != -1);
      ::glUniform1i(location, 0);
      assert(::glGetError() == GL_NO_ERROR);
    }
    if (program == this->vertex_fragment_shader_program_) {
      color_matrix const& mat = this->color_matrix_;
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
      ::glUniformMatrix4fv(glGetUniformLocation(program,
                                                "color_matrix"),
                           1, GL_FALSE, gl_color_mat);
      float const gl_color_mat_translation[] = {
        static_cast<float>(mat.element<0, 4>()),
        static_cast<float>(mat.element<1, 4>()),
        static_cast<float>(mat.element<2, 4>()),
        static_cast<float>(mat.element<3, 4>()),
      };
      ::glUniform4fv(glGetUniformLocation(program,
                                          "color_matrix_translation"),
                     4, gl_color_mat_translation);
    }
  }
  template<class InputIterator>
  void
  set_projection_matrix(InputIterator first, InputIterator last) {
    std::copy(first, last, this->projection_matrix_.data());
  }
  // TODO: Check multi process
  void
  initialize_shader_programs() {
    static std::string const
      vertex_shader_src("uniform mat4 projection_matrix;\n"
                        "uniform mat4 modelview_matrix;\n"
                        "\n"
                        "void main(void) {\n"
                        "  gl_TexCoord[0] = gl_MultiTexCoord0;\n"
                        "  gl_Position = projection_matrix * modelview_matrix * gl_Vertex;\n"
                        "}\n");
    static std::string const
      fragment_shader_src("uniform sampler2D texture;\n"
                          "\n"
                          "void main(void) {\n"
                          //"  gl_FragColor = vec4(0.5, 0.5, 0.5, 0.5);\n"
                          "  gl_FragColor = texture2DProj(texture, gl_TexCoord[0]);\n"
                          "}\n");
    static std::string const
      color_mat_shader_src("uniform sampler2D texture;\n"
                           "uniform mat4 color_matrix;\n"
                           "uniform vec4 color_matrix_translation;\n"
                           "\n"
                           "void main(void) {\n"
                           "  vec4 color = texture2DProj(texture, gl_TexCoord[0]);\n"
                           "  gl_FragColor = color_matrix * color + color_matrix_translation;\n"
                           "}\n");
    // TODO: ARB?
    GLuint vertex_shader = ::glCreateShader(GL_VERTEX_SHADER);
    assert(vertex_shader);
    GLuint fragment_shader = ::glCreateShader(GL_FRAGMENT_SHADER);
    assert(fragment_shader);
    GLuint color_mat_shader = ::glCreateShader(GL_FRAGMENT_SHADER);
    assert(color_mat_shader);
    this->compile_shader(vertex_shader, vertex_shader_src);
    this->compile_shader(fragment_shader, fragment_shader_src);
    this->compile_shader(color_mat_shader, color_mat_shader_src);
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
      this->vertex_shader_program_ = program;
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
      this->vertex_fragment_shader_program_ = program;
    }
    ::glDeleteShader(vertex_shader);
    ::glDeleteShader(fragment_shader);
    ::glDeleteShader(color_mat_shader);
  }
  void
  compile_shader(GLuint shader, std::string const& src) {
    char const* src_p = src.c_str();
    ::glShaderSource(shader, 1, &src_p, 0);
    ::glCompileShader(shader);
    GLint compiled;
    ::glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    this->show_shader_log(shader);
    if (compiled == GL_FALSE) {
      throw std::runtime_error("shader compile error");
    }
  }
  // TODO: Debug Mode
  void
  show_shader_log(GLuint shader) {
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
      throw std::runtime_error(std::string("shader error: ") + buffer.get());
    }
  }
  // TODO: show_program_log
};

}
}
}

#endif
