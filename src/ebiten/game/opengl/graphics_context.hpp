#ifndef EBITEN_GAME_OPENGL_GRAPHICS_CONTEXT_HPP
#define EBITEN_GAME_OPENGL_GRAPHICS_CONTEXT_HPP

#include "ebiten/game/video/color_matrix.hpp"
#include "ebiten/game/video/drawing_region.hpp"
#include "ebiten/game/video/geometry_matrix.hpp"
#include "ebiten/game/video/texture.hpp"
#include "ebiten/util/singleton.hpp"
#include <boost/range.hpp>
#include <GLUT/glut.h>
#include <algorithm>
#include <array>
#include <cassert>

namespace ebiten {
namespace game {
namespace opengl {

class graphics_context : public util::singleton<graphics_context> {
  friend class util::singleton<graphics_context>;
private:
  GLuint shader_program;
public:
  template<class DrawingRegions>
  void
  draw_textures(const video::texture& texture,
                const DrawingRegions& drawing_regions,
                const video::geometry_matrix& geo,
                int z,
                const video::color_matrix&) {
    // TODO: use color matrix
    if (!this->shader_program) {
      this->shader_program = compile_shader_program();
      assert(this->shader_program);
    }
    ::glUseProgram(this->shader_program);

    const float gl_geo[] = {geo.a(),  geo.c(),  0, 0,
                            geo.b(),  geo.d(),  0, 0,
                            0,        0,        1, 0,
                            geo.tx(), geo.ty(), 0, 1};
    ::glMatrixMode(GL_MODELVIEW);
    ::glLoadMatrixf(gl_geo);
    assert(texture.id());
    ::glBindTexture(GL_TEXTURE_2D, texture.id());
    ::glBegin(GL_QUADS);
    const float zf = static_cast<float>(z);
    const float texture_width  = texture.texture_width();
    const float texture_height = texture.texture_height();
    std::for_each(boost::begin(drawing_regions), boost::end(drawing_regions),
                  [&](const video::drawing_region& t) {
                    const float tu1 = t.src_x              / texture_width;
                    const float tu2 = (t.src_x + t.width)  / texture_width;
                    const float tv1 = t.src_y              / texture_height;
                    const float tv2 = (t.src_y + t.height) / texture_height;
                    const float x1 = t.dst_x;
                    const float x2 = t.dst_x + t.width;
                    const float y1 = t.dst_y;
                    const float y2 = t.dst_y + t.height;
                    const float vertex[4][3] = {{x1, y1, zf},
                                                {x2, y1, zf},
                                                {x2, y2, zf},
                                                {x1, y2, zf}};
                    ::glTexCoord2f(tu1, tv1);
                    ::glVertex3fv(vertex[0]);
                    ::glTexCoord2f(tu2, tv1);
                    ::glVertex3fv(vertex[1]);
                    ::glTexCoord2f(tu2, tv2);
                    ::glVertex3fv(vertex[2]);
                    ::glTexCoord2f(tu1, tv2);
                    ::glVertex3fv(vertex[3]);
                  });
    ::glEnd();
    ::glBindTexture(GL_TEXTURE_2D, 0);
    //::glPopMatrix();
  }
private:
  graphics_context()
    : shader_program(0) {
  }
  ~graphics_context() {
  }
  GLuint
  compile_shader_program() {
    const std::string sharder_source("void main(void) {\n"
                                     " hage \n"
                                     "}\n");
    // TODO: ARB?
    GLuint fragment_shader;
    fragment_shader = ::glCreateShader(GL_FRAGMENT_SHADER);
    assert(fragment_shader);
    const char* shader_source_p = sharder_source.c_str();
    ::glShaderSource(fragment_shader, 1, &shader_source_p, nullptr);
    ::glCompileShader(fragment_shader);
    // check status
    GLint compiled;
    ::glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compiled);
    this->show_log(fragment_shader);
    if (compiled == GL_FALSE) {
      throw "shader compile error";
    }

    GLuint program;
    program = ::glCreateProgram();
    assert(program);
    ::glAttachShader(program, fragment_shader);
    ::glDeleteShader(fragment_shader);
    ::glLinkProgram(program);
    // check status
    GLint linked;
    ::glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (linked == GL_FALSE) {
      throw "program error";
    }

    return program;
  }
  void
  show_log(GLuint shader) {
    int log_size = 0;
    int length;
    std::array<char, 1024> buffer;
    ::glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_size);
    if (log_size) {
      // TODO: バッファ確認
      ::glGetShaderInfoLog(shader, buffer.size(), &length, buffer.data());
      std::cerr << buffer.data() << std::endl;
    }
  }
};

}
}
}

#endif
