#ifndef EBITEN_GAME_GRAPHICS_OPENGL_GRAPHICS_CONTEXT_HPP
#define EBITEN_GAME_GRAPHICS_OPENGL_GRAPHICS_CONTEXT_HPP

#include "ebiten/game/graphics/color_matrix.hpp"
#include "ebiten/game/graphics/drawing_region.hpp"
#include "ebiten/game/graphics/geometry_matrix.hpp"
#include "ebiten/game/graphics/texture.hpp"
#include "ebiten/util/singleton.hpp"
#include <OpenGL/gl.h>
#include <boost/range.hpp>
#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>

namespace ebiten {
namespace game {
namespace graphics {
namespace opengl {

class graphics_context : public util::singleton<graphics_context> {
  friend class util::singleton<graphics_context>;
private:
  GLuint shader_program;
public:
  template<class DrawingRegions>
  void
  draw_textures(const graphics::texture& texture,
                const DrawingRegions& drawing_regions,
                const graphics::geometry_matrix& geo_mat,
                int z,
                const graphics::color_matrix& color_mat) {
    if (!this->shader_program) {
      this->shader_program = compile_shader_program();
      assert(this->shader_program);
    }
    if (!color_mat.is_identity()) {
      ::glUseProgram(this->shader_program);
      ::glUniform1i(glGetUniformLocation(this->shader_program, "texture"), 0);
      const float gl_color_mat[] = {
        static_cast<float>(color_mat.element<0, 0>()),
        static_cast<float>(color_mat.element<0, 1>()),
        static_cast<float>(color_mat.element<0, 2>()),
        static_cast<float>(color_mat.element<0, 3>()),
        static_cast<float>(color_mat.element<1, 0>()),
        static_cast<float>(color_mat.element<1, 1>()),
        static_cast<float>(color_mat.element<1, 2>()),
        static_cast<float>(color_mat.element<1, 3>()),
        static_cast<float>(color_mat.element<2, 0>()),
        static_cast<float>(color_mat.element<2, 1>()),
        static_cast<float>(color_mat.element<2, 2>()),
        static_cast<float>(color_mat.element<2, 3>()),
        static_cast<float>(color_mat.element<3, 0>()),
        static_cast<float>(color_mat.element<3, 1>()),
        static_cast<float>(color_mat.element<3, 2>()),
        static_cast<float>(color_mat.element<3, 3>()),
      };
      ::glUniformMatrix4fv(glGetUniformLocation(this->shader_program, "color_matrix"),
                           1, GL_FALSE, gl_color_mat);
      const float gl_color_mat_translation[] = {
        static_cast<float>(color_mat.element<0, 4>()),
        static_cast<float>(color_mat.element<1, 4>()),
        static_cast<float>(color_mat.element<2, 4>()),
        static_cast<float>(color_mat.element<3, 4>()),
      };
      ::glUniform4fv(glGetUniformLocation(this->shader_program, "color_matrix_translation"),
                     4, gl_color_mat_translation);
    }
    const float gl_geo_mat[] = {geo_mat.a(),  geo_mat.c(),  0, 0,
                                geo_mat.b(),  geo_mat.d(),  0, 0,
                                0,            0,            1, 0,
                                geo_mat.tx(), geo_mat.ty(), 0, 1};
    ::glMatrixMode(GL_MODELVIEW);
    ::glLoadMatrixf(gl_geo_mat);
    assert(texture.id());
    ::glBindTexture(GL_TEXTURE_2D, texture.id());
    ::glBegin(GL_QUADS);
    const float zf = static_cast<float>(z);
    const float texture_width  = texture.texture_width();
    const float texture_height = texture.texture_height();
    std::for_each(boost::begin(drawing_regions), boost::end(drawing_regions),
                  [&](const drawing_region& t) {
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
                    // TODO: use glDrawArrays?
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
    ::glUseProgram(0);
  }
private:
  graphics_context()
    : shader_program(0) {
  }
  ~graphics_context() {
  }
  GLuint
  compile_shader_program() {
    const std::string sharder_source("uniform sampler2D texture;\n"
                                     "uniform mat4 color_matrix;\n"
                                     "uniform vec4 color_matrix_translation;\n"
                                     "\n"
                                     "void main(void) {\n"
                                     "  vec4 color = texture2DProj(texture, gl_TexCoord[0]);\n"
                                     "  gl_FragColor = color_matrix * color + color_matrix_translation;\n"
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
    this->show_shader_log(fragment_shader);
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
  // TODO: Debug Mode
  void
  show_shader_log(GLuint shader) {
    int log_size = 0;
    ::glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_size);
    if (log_size) {
      int length = 0;
      // TODO: 動的確保のほうがよい?
      std::array<char, 1024> buffer;
      // TODO: バッファ確認
      ::glGetShaderInfoLog(shader, buffer.size(), &length, buffer.data());
      std::cerr << buffer.data() << std::endl;
    }
  }
  // TODO: show_program_log
};

}
}
}
}

#endif
