#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_SHADERS_HPP
#define EBITEN_GRAPHICS_DETAIL_OPENGL_SHADERS_HPP

#include "ebiten/noncopyable.hpp"
#include "ebiten/platform.hpp"

#ifdef EBITEN_MACOSX
# include <OpenGL/gl.h>
#endif
#ifdef EBITEN_IOS
# import <GLKit/GLKit.h>
#endif

#include <iostream>
#include <regex>
#include <sstream>
#include <string>

namespace ebiten {
namespace graphics {
namespace detail {

class shaders : private noncopyable {
private:
  bool initialized_;
  GLuint regular_shader_program_;
  GLuint color_mat_shader_program_;
public:
  shaders()
    : initialized_(false) {
  }
  // TODO: logging
  GLuint
  regular_shader_program() {
    if (!this->initialized_) {
      try {
        this->initialize();
      } catch (std::runtime_error const& err) {
        std::cerr << err.what() << std::endl;
        throw;
      }
      this->initialized_ = true;
    }
    return this->regular_shader_program_;
  }
  GLuint
  color_mat_shader_program() {
    if (!this->initialized_) {
      try {
        this->initialize();
      } catch (std::runtime_error const& err) {
        std::cerr << err.what() << std::endl;
        throw;
      }
      this->initialized_ = true;
    }
    return this->color_mat_shader_program_;
  }
private:
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
  initialize() {
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
