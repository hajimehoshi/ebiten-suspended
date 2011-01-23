#ifndef EBITEN_GAME_GRAPHICS_COLOR_MATRIX_HPP
#define EBITEN_GAME_GRAPHICS_COLOR_MATRIX_HPP

#include "ebiten/game/graphics/affine_matrix.hpp"

namespace ebiten {
namespace game {
namespace graphics {

template<class Float>
class color_matrix_base : public affine_matrix<Float, 5> {
public:
  color_matrix_base(std::initializer_list<Float> elements_)
    : affine_matrix<Float, 5>(elements_) {
  }
};

typedef color_matrix_base<double> color_matrix;

}
}
}

#endif
