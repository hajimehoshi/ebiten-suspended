#ifndef EBITEN_GRAPHICS_AFFINE_MATRIX_HPP
#define EBITEN_GRAPHICS_AFFINE_MATRIX_HPP

#include <array>
#include <cassert>

namespace ebiten {
namespace graphics {

template<class Float, std::size_t Dimension>
class affine_matrix {
  static_assert(0 < Dimension, "Dimension must be more than 0");
private:
  static std::size_t const size = Dimension * (Dimension - 1);
  typedef std::array<Float, size> elements_type;
  elements_type elements_;
public:
  affine_matrix() {
    this->elements_.fill(0);
  }
  template<std::size_t I, std::size_t J>
  Float
  element() const {
    static_assert(I < Dimension, "I must be less than Dimension");
    static_assert(J < Dimension, "J must be less than Dimension");
    if (I == Dimension - 1) {
      if (J == Dimension - 1) {
        return 1;
      }
      return 0;
    }
    return this->elements_[I * Dimension + J];
  }
  template<std::size_t I, std::size_t J>
  Float
  set_element(Float element) {
    static_assert(I < Dimension - 1, "I must be less than Dimension - 1");
    static_assert(J < Dimension,     "J must be less than Dimension");
    return this->elements_[I * Dimension + J] = element;
  }
  bool
  is_identity() const {
    typename elements_type::const_iterator it = this->elements_.begin();
    for (std::size_t i = 0; i < Dimension - 1; ++i) {
      for (std::size_t j = 0; j < Dimension; ++j, ++it) {
        if (i == j) {
          if (*it != 1) {
            return false;
          }
        } else {
          if (*it != 0) {
            return false;
          }
        }
      }
    }
    return true;
  }
};

}
}

#ifdef EBITEN_TEST

namespace ebiten {
namespace graphics {

BOOST_AUTO_TEST_CASE(affine_matrix_element) {
  affine_matrix<double, 4> m;
  m.set_element<0, 0>(1);
  m.set_element<0, 1>(2);
  m.set_element<0, 2>(3);
  BOOST_CHECK_EQUAL(1, (m.element<0, 0>()));
  BOOST_CHECK_EQUAL(2, (m.element<0, 1>()));
  BOOST_CHECK_EQUAL(3, (m.element<0, 2>()));
  BOOST_CHECK_EQUAL(0, (m.element<0, 3>()));
  BOOST_CHECK_EQUAL(0, (m.element<1, 0>()));
  BOOST_CHECK_EQUAL(0, (m.element<1, 1>()));
  BOOST_CHECK_EQUAL(0, (m.element<1, 2>()));
  BOOST_CHECK_EQUAL(0, (m.element<1, 3>()));
  BOOST_CHECK_EQUAL(0, (m.element<2, 0>()));
  BOOST_CHECK_EQUAL(0, (m.element<2, 1>()));
  BOOST_CHECK_EQUAL(0, (m.element<2, 2>()));
  BOOST_CHECK_EQUAL(0, (m.element<2, 3>()));
  BOOST_CHECK_EQUAL(0, (m.element<3, 0>()));
  BOOST_CHECK_EQUAL(0, (m.element<3, 1>()));
  BOOST_CHECK_EQUAL(0, (m.element<3, 2>()));
  BOOST_CHECK_EQUAL(1, (m.element<3, 3>()));
  m.set_element<1, 0>(4);
  m.set_element<1, 1>(5);
  m.set_element<2, 3>(6);
  BOOST_CHECK_EQUAL(1, (m.element<0, 0>()));
  BOOST_CHECK_EQUAL(2, (m.element<0, 1>()));
  BOOST_CHECK_EQUAL(3, (m.element<0, 2>()));
  BOOST_CHECK_EQUAL(0, (m.element<0, 3>()));
  BOOST_CHECK_EQUAL(4, (m.element<1, 0>()));
  BOOST_CHECK_EQUAL(5, (m.element<1, 1>()));
  BOOST_CHECK_EQUAL(0, (m.element<1, 2>()));
  BOOST_CHECK_EQUAL(0, (m.element<1, 3>()));
  BOOST_CHECK_EQUAL(0, (m.element<2, 0>()));
  BOOST_CHECK_EQUAL(0, (m.element<2, 1>()));
  BOOST_CHECK_EQUAL(0, (m.element<2, 2>()));
  BOOST_CHECK_EQUAL(6, (m.element<2, 3>()));
  BOOST_CHECK_EQUAL(0, (m.element<3, 0>()));
  BOOST_CHECK_EQUAL(0, (m.element<3, 1>()));
  BOOST_CHECK_EQUAL(0, (m.element<3, 2>()));
  BOOST_CHECK_EQUAL(1, (m.element<3, 3>()));
}

BOOST_AUTO_TEST_CASE(affine_matrix_is_identity) {
  affine_matrix<double, 4> m1;
  m1.set_element<0, 0>(1);
  m1.set_element<1, 1>(1);
  m1.set_element<2, 2>(1);
  BOOST_CHECK(m1.is_identity());
  affine_matrix<double, 4> m2 = m1;
  BOOST_CHECK(m2.is_identity());
  m2.set_element<0, 1>(1);
  BOOST_CHECK(!m2.is_identity());
}

}
}

#endif

#endif
