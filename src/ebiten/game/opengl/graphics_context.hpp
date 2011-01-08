#ifndef EBITEN_GAME_OPENGL_GRAPHICS_CONTEXT_HPP
#define EBITEN_GAME_OPENGL_GRAPHICS_CONTEXT_HPP

#include "ebiten/game/video/color_matrix.hpp"
#include "ebiten/game/video/geometry_matrix.hpp"
#include "ebiten/game/video/texture.hpp"
#include "ebiten/util/singleton.hpp"
#include <boost/range.hpp>
#include <GLUT/glut.h>
#include <algorithm>

namespace ebiten {
namespace game {
namespace opengl {

class graphics_context : public util::singleton<graphics_context> {
  friend class util::singleton<graphics_context>;
public:
  template<class TexelRects>
  void
  draw_textures(const video::texture& texture,
                const TexelRects& texel_rects,
                const video::geometry_matrix& geo,
                int z,
                const video::color_matrix&) {
    // TODO: use color matrix
    const float gl_geo[] = {geo.a(),  geo.c(),  0, 0,
                            geo.b(),  geo.d(),  0, 0,
                            0,        0,        1, 0,
                            geo.tx(), geo.ty(), 0, 1};
    ::glMatrixMode(GL_MODELVIEW);
    ::glLoadMatrixf(gl_geo);
    const float width  = static_cast<float>(texture.width());
    const float height = static_cast<float>(texture.height());
    const float tu     = width  / texture.texture_width();
    const float tv     = height / texture.texture_height();
    const float zf     = static_cast<float>(z);
    const float vertex[4][3] = {{0,     0,      zf},
                                {width, 0,      zf},
                                {width, height, zf},
                                {0,     height, zf}};
    assert(texture.id());
    ::glBindTexture(GL_TEXTURE_2D, texture.id());
    ::glBegin(GL_QUADS);
    //std::for_each(boost::begin(texel_rects), boost::end(texel_rects),
    //              [&](const video::texel_rect& t) {
                    ::glTexCoord2f(0, 0);
                    ::glVertex3fv(vertex[0]);
                    ::glTexCoord2f(tu, 0);
                    ::glVertex3fv(vertex[1]);
                    ::glTexCoord2f(tu, tv);
                    ::glVertex3fv(vertex[2]);
                    ::glTexCoord2f(0, tv);
                    ::glVertex3fv(vertex[3]);
    //              });
    ::glEnd();
    ::glBindTexture(GL_TEXTURE_2D, 0);
    //::glPopMatrix();
  }
private:
  graphics_context() {
  }
  ~graphics_context() {
  }
};

}
}
}

#endif
