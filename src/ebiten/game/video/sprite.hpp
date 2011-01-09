#ifndef EBITEN_GAME_VIDEO_SPRITE_HPP
#define EBITEN_GAME_VIDEO_SPRITE_HPP

#include "ebiten/game/video/color_matrix.hpp"
#include "ebiten/game/video/drawing_region.hpp"
#include "ebiten/game/video/geometry_matrix.hpp"
#include "ebiten/game/video/texture.hpp"
#include <boost/noncopyable.hpp>
#include <boost/range.hpp>

namespace ebiten {
namespace game {
namespace video {

class sprite : private boost::noncopyable {
private:
  const texture& texture_;
  typedef std::vector<drawing_region> drawing_regions_type;
  drawing_regions_type drawing_regions_;
  video::geometry_matrix geometry_matrix_;
  double z_;
  video::color_matrix color_matrix_;
  bool is_visible_;
public:
  sprite(const texture& texture_, std::size_t drawing_regions_count)
    : texture_(texture_),
      geometry_matrix_(1, 0, 0, 1, 0, 0),
      z_(0),
      color_matrix_({1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0}),
      is_visible_(true) {
    drawing_regions_.reserve(drawing_regions_count);
    for (std::size_t i = 0; i < drawing_regions_count; ++i) {
      drawing_regions_.emplace_back(0, 0, 0, 0, 0, 0);
    }
  }
  template<class GraphicsContext>
  void
  draw(GraphicsContext& gc) const {
    if (!this->is_visible()) {
      return;
    }
    gc.draw_textures(this->texture_,
                     this->drawing_regions_,
                     this->geometry_matrix_,
                     this->z_,
                     this->color_matrix_);
  }
  video::drawing_region&
  drawing_region_at(std::size_t i) {
    return this->drawing_regions_.at(i);
  }
  const video::drawing_region&
  drawing_region_at(std::size_t i) const {
    return this->drawing_regions_.at(i);
  }
  const drawing_regions_type&
  drawing_regions() const {
    return this->drawing_regions_;
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
