#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_MACOSX_OPENGL_INITIALIZER_HPP
#define EBITEN_GRAPHICS_DETAIL_OPENGL_MACOSX_OPENGL_INITIALIZER_HPP

#include "ebiten/graphics/native_view.hpp"
#include "ebiten/noncopyable.hpp"
#include <functional>

namespace ebiten {
namespace graphics {
namespace detail {

class opengl_initializer : private noncopyable {
public:
  void
  initialize(native_view native_view,
             std::function<void()> const& updating_func) {
    [native_view setUpdatingFunc:updating_func];
  }
};

}
}
}


#endif
