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
  elements_type elements_;
protected:
  affine_matrix() {
    this->elements_.fill(0);
  }
  template<class InputIterator>
  affine_matrix(InputIterator const& begin, InputIterator const& end) {
    std::copy(begin, end, this->elements_.begin());
  }
  virtual
  ~affine_matrix() {
  }
public:
  Float
  element(std::size_t i, std::size_t j) const {
    assert(i < Dimension);
    assert(j < Dimension);
    if (i == Dimension - 1) {
      if (j == Dimension - 1) {
        return 1;
      }
      return 0;
    }
    return this->elements_[i * Dimension + j];
  }
  void
  set_element(std::size_t i, std::size_t j, Float element) {
    assert(i < Dimension - 1);
    assert(j < Dimension);
    this->elements_[i * Dimension + j] = element;
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
  static Self const&
  identity() {
    static Self identity_;
    std::call_once(identity_once_flag_, initialize_identity, std::ref<Self>(identity_));
    return identity_;
  }
  Self
  concat(Self const& other) const {
    Self result;
    elements_type const& lhs_elements = other.elements_;
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
  m.set_element(0, 0, 1);
  m.set_element(0, 1, 2);
  m.set_element(0, 2, 3);
  EXPECT_EQ(1, (m.element(0, 0)));
  EXPECT_EQ(2, (m.element(0, 1)));
  EXPECT_EQ(3, (m.element(0, 2)));
  EXPECT_EQ(0, (m.element(0, 3)));
  EXPECT_EQ(0, (m.element(1, 0)));
  EXPECT_EQ(0, (m.element(1, 1)));
  EXPECT_EQ(0, (m.element(1, 2)));
  EXPECT_EQ(0, (m.element(1, 3)));
  EXPECT_EQ(0, (m.element(2, 0)));
  EXPECT_EQ(0, (m.element(2, 1)));
  EXPECT_EQ(0, (m.element(2, 2)));
  EXPECT_EQ(0, (m.element(2, 3)));
  EXPECT_EQ(0, (m.element(3, 0)));
  EXPECT_EQ(0, (m.element(3, 1)));
  EXPECT_EQ(0, (m.element(3, 2)));
  EXPECT_EQ(1, (m.element(3, 3)));
  m.set_element(1, 0, 4);
  m.set_element(1, 1, 5);
  m.set_element(2, 3, 6);
  EXPECT_EQ(1, (m.element(0, 0)));
  EXPECT_EQ(2, (m.element(0, 1)));
  EXPECT_EQ(3, (m.element(0, 2)));
  EXPECT_EQ(0, (m.element(0, 3)));
  EXPECT_EQ(4, (m.element(1, 0)));
  EXPECT_EQ(5, (m.element(1, 1)));
  EXPECT_EQ(0, (m.element(1, 2)));
  EXPECT_EQ(0, (m.element(1, 3)));
  EXPECT_EQ(0, (m.element(2, 0)));
  EXPECT_EQ(0, (m.element(2, 1)));
  EXPECT_EQ(0, (m.element(2, 2)));
  EXPECT_EQ(6, (m.element(2, 3)));
  EXPECT_EQ(0, (m.element(3, 0)));
  EXPECT_EQ(0, (m.element(3, 1)));
  EXPECT_EQ(0, (m.element(3, 2)));
  EXPECT_EQ(1, (m.element(3, 3)));
}

TEST(affine_matrix, is_identity) {
  affine_matrix_impl<double, 4> m1;
  m1.set_element(0, 0, 1);
  m1.set_element(1, 1, 1);
  m1.set_element(2, 2, 1);
  EXPECT_TRUE(m1.is_identity());
  affine_matrix_impl<double, 4> m2 = m1;
  EXPECT_TRUE(m2.is_identity());
  m2.set_element(0, 1, 1);
  EXPECT_FALSE(m2.is_identity());
}

TEST(affine_matrix, multiply) {
  {
    affine_matrix_impl<double, 3> m1 = affine_matrix_impl<double, 3>::identity();
    affine_matrix_impl<double, 3> m2 = affine_matrix_impl<double, 3>::identity();
    m1.set_element(0, 0, 2);
    m1.set_element(1, 1, 2);
    m2.set_element(0, 2, 1);
    m2.set_element(1, 2, 1);
    {
      affine_matrix_impl<double, 3> m3 = m1.concat(m2);
      EXPECT_EQ(2, (m3.element(0, 0)));
      EXPECT_EQ(0, (m3.element(0, 1)));
      EXPECT_EQ(1, (m3.element(0, 2)));
      EXPECT_EQ(0, (m3.element(1, 0)));
      EXPECT_EQ(2, (m3.element(1, 1)));
      EXPECT_EQ(1, (m3.element(1, 2)));
      EXPECT_EQ(0, (m3.element(2, 0)));
      EXPECT_EQ(0, (m3.element(2, 1)));
      EXPECT_EQ(1, (m3.element(2, 2)));
    }
    {
      affine_matrix_impl<double, 3> m3 = m2.concat(m1);
      EXPECT_EQ(2, (m3.element(0, 0)));
      EXPECT_EQ(0, (m3.element(0, 1)));
      EXPECT_EQ(2, (m3.element(0, 2)));
      EXPECT_EQ(0, (m3.element(1, 0)));
      EXPECT_EQ(2, (m3.element(1, 1)));
      EXPECT_EQ(2, (m3.element(1, 2)));
      EXPECT_EQ(0, (m3.element(2, 0)));
      EXPECT_EQ(0, (m3.element(2, 1)));
      EXPECT_EQ(1, (m3.element(2, 2)));
    }
  }
  {
    affine_matrix_impl<double, 4> m1;
    affine_matrix_impl<double, 4> m2;
    m1.set_element(0, 0, 1);
    m1.set_element(0, 1, 2);
    m1.set_element(0, 2, 3);
    m1.set_element(0, 3, 4);
    m1.set_element(1, 0, 5);
    m1.set_element(1, 1, 6);
    m1.set_element(1, 2, 7);
    m1.set_element(1, 3, 8);
    m1.set_element(2, 0, 9);
    m1.set_element(2, 1, 10);
    m1.set_element(2, 2, 11);
    m1.set_element(2, 3, 12);
    m2.set_element(0, 0, 13);
    m2.set_element(0, 1, 14);
    m2.set_element(0, 2, 15);
    m2.set_element(0, 3, 16);
    m2.set_element(1, 0, 17);
    m2.set_element(1, 1, 18);
    m2.set_element(1, 2, 19);
    m2.set_element(1, 3, 20);
    m2.set_element(2, 0, 21);
    m2.set_element(2, 1, 22);
    m2.set_element(2, 2, 23);
    m2.set_element(2, 3, 24);
    {
      affine_matrix_impl<double, 4> m3 = m1.concat(m2);
      EXPECT_EQ(218, (m3.element(0, 0)));
      EXPECT_EQ(260, (m3.element(0, 1)));
      EXPECT_EQ(302, (m3.element(0, 2)));
      EXPECT_EQ(360, (m3.element(0, 3)));
      EXPECT_EQ(278, (m3.element(1, 0)));
      EXPECT_EQ(332, (m3.element(1, 1)));
      EXPECT_EQ(386, (m3.element(1, 2)));
      EXPECT_EQ(460, (m3.element(1, 3)));
      EXPECT_EQ(338, (m3.element(2, 0)));
      EXPECT_EQ(404, (m3.element(2, 1)));
      EXPECT_EQ(470, (m3.element(2, 2)));
      EXPECT_EQ(560, (m3.element(2, 3)));
      EXPECT_EQ(0,   (m3.element(3, 0)));
      EXPECT_EQ(0,   (m3.element(3, 1)));
      EXPECT_EQ(0,   (m3.element(3, 2)));
      EXPECT_EQ(1,   (m3.element(3, 3)));
    }
    {
      affine_matrix_impl<double, 4> m3 = m2.concat(m1);
      EXPECT_EQ(110, (m3.element(0, 0)));
      EXPECT_EQ(116, (m3.element(0, 1)));
      EXPECT_EQ(122, (m3.element(0, 2)));
      EXPECT_EQ(132, (m3.element(0, 3)));
      EXPECT_EQ(314, (m3.element(1, 0)));
      EXPECT_EQ(332, (m3.element(1, 1)));
      EXPECT_EQ(350, (m3.element(1, 2)));
      EXPECT_EQ(376, (m3.element(1, 3)));
      EXPECT_EQ(518, (m3.element(2, 0)));
      EXPECT_EQ(548, (m3.element(2, 1)));
      EXPECT_EQ(578, (m3.element(2, 2)));
      EXPECT_EQ(620, (m3.element(2, 3)));
      EXPECT_EQ(0,   (m3.element(3, 0)));
      EXPECT_EQ(0,   (m3.element(3, 1)));
      EXPECT_EQ(0,   (m3.element(3, 2)));
      EXPECT_EQ(1,   (m3.element(3, 3)));
    }
  }
}

}
}

#endif

#endif
