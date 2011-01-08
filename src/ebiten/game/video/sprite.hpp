#ifndef EBITEN_GAME_VIDEO_SPRITE_HPP
#define EBITEN_GAME_VIDEO_SPRITE_HPP

#include "ebiten/game/video/color_matrix.hpp"
#include "ebiten/game/video/geometry_matrix.hpp"
#include "ebiten/game/video/texture.hpp"
#include <boost/noncopyable.hpp>
#include <boost/range.hpp>

namespace ebiten {
namespace game {
namespace video {

struct texel_rect {
  std::size_t x;
  std::size_t y;
  std::size_t width;
  std::size_t height;
  std::size_t texel_x;
  std::size_t texel_y;
  std::size_t texel_width;
  std::size_t texel_height;
};

class sprite : private boost::noncopyable {
private:
  const texture& texture_;
  typedef std::vector<texel_rect> texel_rects_type;
  texel_rects_type texel_rects_;
  video::geometry_matrix geometry_matrix_;
  double z_;
  video::color_matrix color_matrix_;
  bool is_visible_;
public:
  template<class TexelRects>
  sprite(const texture& texture_, const TexelRects& texel_rects_)
    : texture_(texture_),
      texel_rects_(boost::begin(texel_rects_), boost::end(texel_rects_)),
      geometry_matrix_(1, 0, 0, 1, 0, 0),
      z_(0),
      color_matrix_{1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0},
      is_visible_(true) {
  }
  template<class GraphicsContext>
  void
  draw(GraphicsContext& gc) const {
    if (!this->is_visible()) {
      return;
    }
    gc.draw_textures(this->texture_,
                     this->texel_rects_,
                     this->geometry_matrix_,
                     this->z_,
                     this->color_matrix_);
  }
  video::texel_rect&
  texel_rect_at(std::size_t i) {
    return this->texel_rects_.at(i);
  }
  const video::texel_rect&
  texel_rect_at(std::size_t i) const {
    return this->texel_rects_.at(i);
  }
  const texel_rects_type&
  texel_rects() const {
    return this->texel_rects_;
  }
  video::geometry_matrix&
  geometry_matrix() {
    return this->geometry_matrix_;
  }
  const video::geometry_matrix&
  geometry_matrix() const {
    return this->geometry_matrix_;
  }
  bool
  is_visible() const {
    return this->is_visible_;
  }
  void
  set_visible(bool is_visible) {
    this->is_visible_ = is_visible;
  }
  double
  z() const {
    return this->z_;
  }
};

}
}
}

#endif
