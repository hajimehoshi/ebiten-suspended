#ifndef EBITEN_GAME_VIDEO_AFFINE_MATRIX_HPP
#define EBITEN_GAME_VIDEO_AFFINE_MATRIX_HPP

#include <boost/mpl/int.hpp>
#include <boost/noncopyable.hpp>
#include <boost/range.hpp>
#include <boost/static_assert.hpp>
#include <algorithm>
#include <array>
#include <cassert>
#include <initializer_list>

namespace ebiten {
namespace game {
namespace video {

template<class Float, int Dimension>
class affine_matrix : private boost::noncopyable {
  BOOST_STATIC_ASSERT(0 < Dimension);
private:
  static const std::size_t size = Dimension * (Dimension - 1);
  std::array<Float, size> elements_;
public:
  // TODO: accepts iterators
  affine_matrix(const std::initializer_list<Float>& elements_) {
    assert(static_cast<std::size_t>(boost::size(elements_)) <= size);
    this->elements_.fill(0);
    std::copy(boost::begin(elements_), boost::end(elements_), this->elements_.begin());
  }
  template<int I, int J>
  Float
  element() const {
    BOOST_STATIC_ASSERT(0 <= I);
    BOOST_STATIC_ASSERT(I < Dimension);
    BOOST_STATIC_ASSERT(0 <= J);
    BOOST_STATIC_ASSERT(J < Dimension);
    return this->element(boost::mpl::int_<I>(), boost::mpl::int_<J>());
  }
  template<int I, int J>
  Float
  set_element(Float element) {
    BOOST_STATIC_ASSERT(0 <= I);
    BOOST_STATIC_ASSERT(I < Dimension - 1);
    BOOST_STATIC_ASSERT(0 <= J);
    BOOST_STATIC_ASSERT(J < Dimension);
    return this->elements_[I * Dimension + J] = element;
  }
private:
  Float
  element(int i, int j) const {
    return this->elements_[i * Dimension + j];
  }
  Float
  element(boost::mpl::int_<Dimension - 1>, int) const {
    return 0;
  }
  Float
  element(boost::mpl::int_<Dimension - 1>, boost::mpl::int_<Dimension - 1>) const {
    return 1;
  }
};

}
}
}

#ifdef EBITEN_TEST

#include <boost/test/unit_test.hpp>

namespace ebiten {
namespace game {
namespace video {

BOOST_AUTO_TEST_CASE(test_affine_matrix) {
  affine_matrix<double, 4> m{1, 2, 3};
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

}
}
}

#endif

#endif
