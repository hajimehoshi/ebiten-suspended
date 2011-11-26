#ifndef EBITEN_GAME_GRAPHICS_SPRITE_HPP
#define EBITEN_GAME_GRAPHICS_SPRITE_HPP

#include "ebiten/game/graphics/color_matrix.hpp"
#include "ebiten/game/graphics/drawing_region.hpp"
#include "ebiten/game/graphics/geometry_matrix.hpp"
#include "ebiten/game/graphics/texture.hpp"
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/range.hpp>

namespace ebiten {
namespace game {
namespace graphics {

class sprite : private boost::noncopyable {
private:
  texture const& texture_;
  typedef boost::ptr_vector<drawing_region> drawing_regions_type;
  drawing_regions_type drawing_regions_;
  graphics::geometry_matrix geometry_matrix_;
  double z_;
  graphics::color_matrix color_matrix_;
  bool is_visible_;
public:
  sprite(texture const& texture_, std::size_t drawing_regions_count)
    : texture_(texture_),
      geometry_matrix_(1, 0, 0, 1, 0, 0),
      z_(0),
      color_matrix_((double[]){1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0}),
      is_visible_(true) {
    drawing_regions_.reserve(drawing_regions_count);
    for (std::size_t i = 0; i < drawing_regions_count; ++i) {
      drawing_regions_.push_back(new drawing_region());
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
  graphics::drawing_region&
  drawing_region_at(std::size_t i) {
    return this->drawing_regions_.at(i);
  }
  graphics::drawing_region const&
  drawing_region_at(std::size_t i) const {
    return this->drawing_regions_.at(i);
  }
  drawing_regions_type &
  drawing_regions() {
    return this->drawing_regions_;
  }
  drawing_regions_type const&
  drawing_regions() const {
    return this->drawing_regions_;
  }
  graphics::geometry_matrix&
  geometry_matrix() {
    return this->geometry_matrix_;
  }
  graphics::geometry_matrix const&
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
  graphics::color_matrix&
  color_matrix() {
    return this->color_matrix_;
  }
  graphics::color_matrix const&
  color_matrix() const {
    return this->color_matrix_;
  }
};

}
}
}

#endif
