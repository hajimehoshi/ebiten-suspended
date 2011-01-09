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

namespace ebiten {
namespace game {
namespace opengl {

class graphics_context : public util::singleton<graphics_context> {
  friend class util::singleton<graphics_context>;
public:
  template<class DrawingRegions>
  void
  draw_textures(const video::texture& texture,
                const DrawingRegions& drawing_regions,
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
  graphics_context() {
  }
  ~graphics_context() {
  }
};

}
}
}

#endif
