#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_MACOSX_OPENGL_INITIALIZER_HPP
#define EBITEN_GRAPHICS_DETAIL_OPENGL_MACOSX_OPENGL_INITIALIZER_HPP

#include "ebiten/graphics/view.hpp"
#include "ebiten/noncopyable.hpp"
#include <functional>

namespace ebiten {
namespace graphics {
namespace detail {

class opengl_initializer : private noncopyable {
public:
  void
  initialize(view& view,
             std::function<void()> const& updating_func) {
    [view.native_view() setUpdatingFunc:updating_func];
  }
};

}
}
}


#endif
