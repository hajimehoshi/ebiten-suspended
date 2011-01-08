#ifndef EBITEN_GAME_VIDEO_GEOMETRY_MATRIX_HPP
#define EBITEN_GAME_VIDEO_GEOMETRY_MATRIX_HPP

#include "ebiten/game/video/affine_matrix.hpp"

namespace ebiten {
namespace game {
namespace video {

template<class Float>
class geometry_matrix_base : public affine_matrix<Float, 3> {
public:
  geometry_matrix_base(Float a, Float b, Float c, Float d, Float tx, Float ty)
    : affine_matrix<Float, 3>{a, b, tx, c, d, ty} {
  }
  Float
  a() const {
    return this->template element<0, 0>();
  }
  Float
  b() const {
    return this->template element<0, 1>();
  }
  Float
  c() const {
    return this->template element<1, 0>();
  }
  Float
  d() const {
    return this->template element<1, 1>();
  }
  Float
  tx() const {
    return this->template element<0, 2>();
  }
  Float
  ty() const {
    return this->template element<1, 2>();
  }
  void
  set_a(Float a) {
    this->template set_element<0, 0>(a);
  }
  void
  set_b(Float b) {
    this->template set_element<0, 1>(b);
  }
  void
  set_c(Float c) {
    this->template set_element<1, 0>(c);
  }
  void
  set_d(Float d) {
    this->template set_element<1, 1>(d);
  }
  void
  set_tx(Float tx) {
    this->template set_element<0, 2>(tx);
  }
  void
  set_ty(Float ty) {
    this->template set_element<1, 2>(ty);
  }
};

typedef geometry_matrix_base<float> geometry_matrix;

}
}
}

#ifdef EBITEN_TEST

#include <boost/test/unit_test.hpp>

namespace ebiten {
namespace game {
namespace video {

BOOST_AUTO_TEST_CASE(test_geometry_matrix) {
  geometry_matrix m{1, 2, 3, 4, 5, 6};
  BOOST_CHECK_EQUAL(1, m.a());
  BOOST_CHECK_EQUAL(2, m.b());
  BOOST_CHECK_EQUAL(3, m.c());
  BOOST_CHECK_EQUAL(4, m.d());
  BOOST_CHECK_EQUAL(5, m.tx());
  BOOST_CHECK_EQUAL(6, m.ty());
  m.set_a(7);
  m.set_b(8);
  m.set_c(9);
  m.set_d(10);
  m.set_tx(11);
  m.set_ty(12);
  BOOST_CHECK_EQUAL(7,  m.a());
  BOOST_CHECK_EQUAL(8,  m.b());
  BOOST_CHECK_EQUAL(9,  m.c());
  BOOST_CHECK_EQUAL(10, m.d());
  BOOST_CHECK_EQUAL(11, m.tx());
  BOOST_CHECK_EQUAL(12, m.ty());
}

}
}
}

#endif

#endif
