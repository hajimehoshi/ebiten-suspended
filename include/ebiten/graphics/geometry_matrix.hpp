#ifndef EBITEN_GRAPHICS_GEOMETRY_MATRIX_HPP
#define EBITEN_GRAPHICS_GEOMETRY_MATRIX_HPP

#include "ebiten/graphics/affine_matrix.hpp"
#include <algorithm>

namespace ebiten {
namespace graphics {

template<class Float>
class geometry_matrix_base : public affine_matrix<Float, 3, geometry_matrix_base<Float> > {
public:
  geometry_matrix_base()
    : affine_matrix<Float, 3, geometry_matrix_base<Float> >() {
  }
  geometry_matrix_base(Float a, Float b, Float c, Float d, Float tx, Float ty)
    : affine_matrix<Float, 3, geometry_matrix_base<Float> >() {
    this->set_a(a);
    this->set_b(b);
    this->set_c(c);
    this->set_d(d);
    this->set_tx(tx);
    this->set_ty(ty);
  }
  Float
  a() const {
    return this->element(0, 0);
  }
  Float
  b() const {
    return this->element(0, 1);
  }
  Float
  c() const {
    return this->element(1, 0);
  }
  Float
  d() const {
    return this->element(1, 1);
  }
  Float
  tx() const {
    return this->element(0, 2);
  }
  Float
  ty() const {
    return this->element(1, 2);
  }
  void
  set_a(Float a) {
    this->set_element(0, 0, a);
  }
  void
  set_b(Float b) {
    this->set_element(0, 1, b);
  }
  void
  set_c(Float c) {
    this->set_element(1, 0, c);
  }
  void
  set_d(Float d) {
    this->set_element(1, 1, d);
  }
  void
  set_tx(Float tx) {
    this->set_element(0, 2, tx);
  }
  void
  set_ty(Float ty) {
    this->set_element(1, 2, ty);
  }
};

typedef geometry_matrix_base<double> geometry_matrix;

}
}

#ifdef EBITEN_TEST

namespace ebiten {
namespace graphics {

TEST(geometry_matrix, member_functions) {
  geometry_matrix m(1, 2, 3, 4, 5, 6);
  EXPECT_EQ(1, m.a());
  EXPECT_EQ(2, m.b());
  EXPECT_EQ(3, m.c());
  EXPECT_EQ(4, m.d());
  EXPECT_EQ(5, m.tx());
  EXPECT_EQ(6, m.ty());
  m.set_a(7);
  m.set_b(8);
  m.set_c(9);
  m.set_d(10);
  m.set_tx(11);
  m.set_ty(12);
  EXPECT_EQ(7,  m.a());
  EXPECT_EQ(8,  m.b());
  EXPECT_EQ(9,  m.c());
  EXPECT_EQ(10, m.d());
  EXPECT_EQ(11, m.tx());
  EXPECT_EQ(12, m.ty());
}

}
}

#endif

#endif
