#ifndef EBITEN_SCRIPT_SPRITE_HPP
#define EBITEN_SCRIPT_SPRITE_HPP

#include "ebiten/graphics/color_matrix.hpp"
#include "ebiten/graphics/geometry_matrix.hpp"
#include "ebiten/graphics/texture.hpp"
#include "ebiten/noncopyable.hpp"

namespace ebiten_script {

class sprite : private ebiten::noncopyable {
private:
  ebiten::graphics::texture texture_;
  int src_x_, src_y_, src_width_, src_height_;
  int x_, y_;
  ebiten::graphics::geometry_matrix geometry_matrix_;
  double alpha_;
  ebiten::graphics::color_matrix color_matrix_;
  bool is_visible_;
public:
  sprite()
    : src_x_(0),
      src_y_(0),
      src_width_(0),
      src_height_(0),
      x_(0),
      y_(0),
      geometry_matrix_(ebiten::graphics::geometry_matrix::identity()),
      alpha_(1),
      color_matrix_(ebiten::graphics::color_matrix::identity()),
      is_visible_(true) {
  }
  ebiten::graphics::texture&
  texture() {
    return this->texture_;
  }
  ebiten::graphics::texture const&
  texture() const {
    return this->texture_;
  }
  void
  set_texture(ebiten::graphics::texture& texture) {
    this->texture_ = texture;
  }
#define DEFINE_PROPERTY(type, name) \
  type name() const { return this->name##_; } \
  void set_##name(type name) { this->name##_ = name; }
  DEFINE_PROPERTY(int, src_x)
  DEFINE_PROPERTY(int, src_y)
  DEFINE_PROPERTY(int, src_width)
  DEFINE_PROPERTY(int, src_height)
  DEFINE_PROPERTY(int, x)
  DEFINE_PROPERTY(int, y)
  DEFINE_PROPERTY(double, alpha)
#undef DEFINE_PROPERTY
  ebiten::graphics::geometry_matrix&
  geometry_matrix() {
    return this->geometry_matrix_;
  }
  ebiten::graphics::geometry_matrix const&
  geometry_matrix() const {
    return this->geometry_matrix_;
  }
  void
  set_geometry_matrix(ebiten::graphics::geometry_matrix const& geometry_matrix) {
    this->geometry_matrix_ = geometry_matrix;
  }
  ebiten::graphics::color_matrix&
  color_matrix() {
    return this->color_matrix_;
  }
  ebiten::graphics::color_matrix const&
  color_matrix() const {
    return this->color_matrix_;
  }
  void
  set_color_matrix(ebiten::graphics::color_matrix const& color_matrix) {
    this->color_matrix_ = color_matrix;
  }
  ebiten::graphics::geometry_matrix
  final_geometry_matrix() const {
    ebiten::graphics::geometry_matrix mat = this->geometry_matrix_;
    mat.set_tx(mat.tx() + this->x_);
    mat.set_ty(mat.ty() + this->y_);
    return mat;
  }
  ebiten::graphics::color_matrix
  final_color_matrix() const {
    ebiten::graphics::color_matrix mat = this->color_matrix_;
    if (this->alpha_ != 1) {
      ebiten::graphics::color_matrix alpha_mat = ebiten::graphics::color_matrix::identity();
      alpha_mat.set_element<3, 3>(this->alpha_);
      mat.concat(alpha_mat);
    }
    return mat;
  }
  bool
  is_visible() const {
    return this->is_visible_;
  }
  void
  set_visible(bool is_visible) {
    this->is_visible_ = is_visible;
  }
};

}

#endif
