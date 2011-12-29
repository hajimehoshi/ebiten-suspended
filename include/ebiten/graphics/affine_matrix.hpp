#ifndef EBITEN_GRAPHICS_AFFINE_MATRIX_HPP
#define EBITEN_GRAPHICS_AFFINE_MATRIX_HPP

#include <array>
#include <cassert>
#include <mutex>

namespace ebiten {
namespace graphics {

/*
 * | e00, e01, ..., e0D |
 * | e10, e11, ..., e1D |
 * |  : ,  : ,    ,  :  |
 * | eD0, eD1, ..., eDD |
 */
template<class Float, std::size_t Dimension, class Self>
class affine_matrix {
  static_assert(0 < Dimension, "Dimension must be more than 0");
#ifdef EBITEN_TEST
private:
  FRIEND_TEST(affine_matrix, element);
  FRIEND_TEST(affine_matrix, is_identity);
  FRIEND_TEST(affine_matrix, multiply);
#endif
private:
  static std::size_t const size_ = Dimension * (Dimension - 1);
  typedef std::array<Float, size_> elements_type;
  static std::once_flag identity_once_flag_;
protected:
  elements_type elements_;
protected:
  affine_matrix() {
    this->elements_.fill(0);
  }
public:
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
  static Self
  identity() {
    static Self identity_;
    std::call_once(identity_once_flag_, initialize_identity, std::ref<Self>(identity_));
    return identity_;
  }
  Self
  multiply(Self const& lhs) const {
    Self result;
    elements_type const& lhs_elements = lhs.elements_;
    elements_type const& rhs_elements = this->elements_;
    typename elements_type::iterator it = result.elements_.begin();
    for (std::size_t i = 0; i < Dimension - 1; ++i) {
      for (std::size_t j = 0; j < Dimension; ++j, ++it) {
        Float e = 0;
        for (std::size_t k = 0; k < Dimension - 1; ++k) {
          e += lhs_elements[i * Dimension + k] * rhs_elements[k * Dimension + j];
        }
        if (j == Dimension - 1) {
          e += lhs_elements[i * Dimension + Dimension - 1];
        }
        *it = e;
      }
    }
    return result;
  }
private:
  static void
  initialize_identity(Self& identity) {
    typename elements_type::iterator it = identity.elements_.begin();
    for (std::size_t i = 0; i < Dimension - 1; ++i) {
      for (std::size_t j = 0; j < Dimension; ++j, ++it) {
        if (i == j) {
          *it = 1;
        } else {
          *it = 0;
        }
      }
    }
  }
};

template<class Float, std::size_t Dimension, class Self>
std::once_flag affine_matrix<Float, Dimension, Self>::identity_once_flag_;

}
}

#ifdef EBITEN_TEST

namespace ebiten {
namespace graphics {

template<class Float, std::size_t Dimension>
class affine_matrix_impl : public affine_matrix<Float,
                                                Dimension,
                                                affine_matrix_impl<Float, Dimension>> {
};

TEST(affine_matrix, element) {
  affine_matrix_impl<double, 4> m;
  m.set_element<0, 0>(1);
  m.set_element<0, 1>(2);
  m.set_element<0, 2>(3);
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
  affine_matrix_impl<double, 4> m1;
  m1.set_element<0, 0>(1);
  m1.set_element<1, 1>(1);
  m1.set_element<2, 2>(1);
  EXPECT_TRUE(m1.is_identity());
  affine_matrix_impl<double, 4> m2 = m1;
  EXPECT_TRUE(m2.is_identity());
  m2.set_element<0, 1>(1);
  EXPECT_FALSE(m2.is_identity());
}

TEST(affine_matrix, multiply) {
  {
    affine_matrix_impl<double, 3> m1 = affine_matrix_impl<double, 3>::identity();
    affine_matrix_impl<double, 3> m2 = affine_matrix_impl<double, 3>::identity();
    m1.set_element<0, 0>(2);
    m1.set_element<1, 1>(2);
    m2.set_element<0, 2>(1);
    m2.set_element<1, 2>(1);
    {
      affine_matrix_impl<double, 3> m3 = m1.multiply(m2);
      EXPECT_EQ(2, (m3.element<0, 0>()));
      EXPECT_EQ(0, (m3.element<0, 1>()));
      EXPECT_EQ(1, (m3.element<0, 2>()));
      EXPECT_EQ(0, (m3.element<1, 0>()));
      EXPECT_EQ(2, (m3.element<1, 1>()));
      EXPECT_EQ(1, (m3.element<1, 2>()));
      EXPECT_EQ(0, (m3.element<2, 0>()));
      EXPECT_EQ(0, (m3.element<2, 1>()));
      EXPECT_EQ(1, (m3.element<2, 2>()));
    }
    {
      affine_matrix_impl<double, 3> m3 = m2.multiply(m1);
      EXPECT_EQ(2, (m3.element<0, 0>()));
      EXPECT_EQ(0, (m3.element<0, 1>()));
      EXPECT_EQ(2, (m3.element<0, 2>()));
      EXPECT_EQ(0, (m3.element<1, 0>()));
      EXPECT_EQ(2, (m3.element<1, 1>()));
      EXPECT_EQ(2, (m3.element<1, 2>()));
      EXPECT_EQ(0, (m3.element<2, 0>()));
      EXPECT_EQ(0, (m3.element<2, 1>()));
      EXPECT_EQ(1, (m3.element<2, 2>()));
    }
  }
  {
    affine_matrix_impl<double, 4> m1;
    affine_matrix_impl<double, 4> m2;
  }
}

}
}

#endif

#endif
