#ifndef EBITEN_GAME_VIDEO_AFFINE_MATRIX_HPP
#define EBITEN_GAME_VIDEO_AFFINE_MATRIX_HPP

#include <boost/mpl/size_t.hpp>
#include <boost/noncopyable.hpp>
#include <boost/range.hpp>
#include <boost/static_assert.hpp>
#include <algorithm>
#include <array>
#include <cassert>
#include <initializer_list>
#include <iterator>

namespace ebiten {
namespace game {
namespace video {

template<class Float, std::size_t Dimension>
class affine_matrix : private boost::noncopyable {
  BOOST_STATIC_ASSERT(0 < Dimension);
private:
  static const std::size_t size = Dimension * (Dimension - 1);
  std::array<Float, size> elements_;
public:
  // TODO: accepts iterators
  // TODO: constructor's arugments?
  affine_matrix(const std::initializer_list<Float>& elements_) {
    assert(static_cast<std::size_t>(boost::size(elements_)) <= size);
    this->elements_.fill(0);
    std::copy(boost::begin(elements_), boost::end(elements_), this->elements_.begin());
  }
  template<std::size_t I, std::size_t J>
  Float
  element() const {
    BOOST_STATIC_ASSERT(I < Dimension);
    BOOST_STATIC_ASSERT(J < Dimension);
    return this->element(boost::mpl::size_t<I>(), boost::mpl::size_t<J>());
  }
  template<std::size_t I, std::size_t J>
  Float
  set_element(Float element) {
    BOOST_STATIC_ASSERT(I < Dimension - 1);
    BOOST_STATIC_ASSERT(J < Dimension);
    return this->elements_[I * Dimension + J] = element;
  }
  bool
  is_identity() const {
    auto it = this->elements_.cbegin();
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
private:
  Float
  element(std::size_t i, std::size_t j) const {
    return this->elements_[i * Dimension + j];
  }
  Float
  element(boost::mpl::size_t<Dimension - 1>, std::size_t) const {
    return 0;
  }
  Float
  element(boost::mpl::size_t<Dimension - 1>, boost::mpl::size_t<Dimension - 1>) const {
    return 1;
  }
};

}
}
}

#ifdef EBITEN_TEST

namespace ebiten {
namespace game {
namespace video {

TEST(affine_matrix, element) {
  affine_matrix<double, 4> m{1, 2, 3};
  EXPECT_EQ(1, (m.element<0, 0>()));
  EXPECT_EQ(2, (m.element<0, 1>()));
  EXPECT_EQ(3, (m.element<0, 2>()));
  EXPECT_EQ(0, (m.element<0, 3>()));
  EXPECT_EQ(0, (m.element<1, 0>()));
  EXPECT_EQ(0, (m.element<1, 1>()));
  EXPECT_EQ(0, (m.element<1, 2>()));
  EXPECT_EQ(0, (m.element<1, 3>()));
  EXPECT_EQ(0, (m.element<2, 0>()));
  EXPECT_EQ(0, (m.element<2, 1>()));
  EXPECT_EQ(0, (m.element<2, 2>()));
  EXPECT_EQ(0, (m.element<2, 3>()));
  EXPECT_EQ(0, (m.element<3, 0>()));
  EXPECT_EQ(0, (m.element<3, 1>()));
  EXPECT_EQ(0, (m.element<3, 2>()));
  EXPECT_EQ(1, (m.element<3, 3>()));
  m.set_element<1, 0>(4);
  m.set_element<1, 1>(5);
  m.set_element<2, 3>(6);
  EXPECT_EQ(1, (m.element<0, 0>()));
  EXPECT_EQ(2, (m.element<0, 1>()));
  EXPECT_EQ(3, (m.element<0, 2>()));
  EXPECT_EQ(0, (m.element<0, 3>()));
  EXPECT_EQ(4, (m.element<1, 0>()));
  EXPECT_EQ(5, (m.element<1, 1>()));
  EXPECT_EQ(0, (m.element<1, 2>()));
  EXPECT_EQ(0, (m.element<1, 3>()));
  EXPECT_EQ(0, (m.element<2, 0>()));
  EXPECT_EQ(0, (m.element<2, 1>()));
  EXPECT_EQ(0, (m.element<2, 2>()));
  EXPECT_EQ(6, (m.element<2, 3>()));
  EXPECT_EQ(0, (m.element<3, 0>()));
  EXPECT_EQ(0, (m.element<3, 1>()));
  EXPECT_EQ(0, (m.element<3, 2>()));
  EXPECT_EQ(1, (m.element<3, 3>()));
}

TEST(affine_matrix, is_identity) {
  affine_matrix<double, 4> m1{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0};
  EXPECT_TRUE(m1.is_identity());
  affine_matrix<double, 4> m2{1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0};
  EXPECT_FALSE(m2.is_identity());
}

}
}
}

#endif

#endif
