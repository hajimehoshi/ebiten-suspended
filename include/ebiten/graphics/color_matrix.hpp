#ifndef EBITEN_GRAPHICS_COLOR_MATRIX_HPP
#define EBITEN_GRAPHICS_COLOR_MATRIX_HPP

#include "ebiten/graphics/affine_matrix.hpp"

namespace ebiten {
namespace graphics {

template<class Float>
class color_matrix_base : public affine_matrix<Float, 5, color_matrix_base<Float> > {
public:
  color_matrix_base()
    : affine_matrix<Float, 5, color_matrix_base<Float> >() {
  }
};

typedef color_matrix_base<double> color_matrix;

}
}

#endif
