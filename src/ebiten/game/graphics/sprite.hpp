#ifndef EBITEN_GAME_GRAPHICS_SPRITE_HPP
#define EBITEN_GAME_GRAPHICS_SPRITE_HPP

#include "ebiten/game/graphics/color_matrix.hpp"
#include "ebiten/game/graphics/drawing_region.hpp"
#include "ebiten/game/graphics/geometry_matrix.hpp"
#include "ebiten/game/graphics/texture.hpp"
#include <boost/make_shared.hpp>
#include <boost/noncopyable.hpp>
#include <boost/range.hpp>

namespace ebiten {
namespace game {
namespace graphics {

class sprite : private boost::noncopyable {
private:
  const texture& texture_;
  typedef boost::shared_ptr<drawing_region> drawing_region_ptr_type;
  typedef std::vector<drawing_region_ptr_type> drawing_regions_type;
  drawing_regions_type drawing_regions_;
  graphics::geometry_matrix geometry_matrix_;
  double z_;
  graphics::color_matrix color_matrix_;
  bool is_visible_;
public:
  sprite(const texture& texture_, std::size_t drawing_regions_count)
    : texture_(texture_),
      geometry_matrix_(1, 0, 0, 1, 0, 0),
      z_(0),
      color_matrix_((double[]){1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0}),
      is_visible_(true) {
    drawing_regions_.reserve(drawing_regions_count);
    for (std::size_t i = 0; i < drawing_regions_count; ++i) {
      drawing_regions_.push_back(boost::make_shared<drawing_region>());
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
    return *this->drawing_regions_.at(i);
  }
  const graphics::drawing_region&
  drawing_region_at(std::size_t i) const {
    return *this->drawing_regions_.at(i);
  }
  const drawing_regions_type&
  drawing_regions() const {
    return this->drawing_regions_;
  }
  graphics::geometry_matrix&
  geometry_matrix() {
    return this->geometry_matrix_;
  }
  const graphics::geometry_matrix&
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
  const graphics::color_matrix&
  color_matrix() const {
    return this->color_matrix_;
  }
};

}
}
}

#endif
