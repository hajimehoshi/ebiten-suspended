#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_MACOSX_OPENGL_INITIALIZER_HPP
#define EBITEN_GRAPHICS_DETAIL_OPENGL_MACOSX_OPENGL_INITIALIZER_HPP

#include "ebiten/graphics/native_view.hpp"
#include "ebiten/noncopyable.hpp"
#include <functional>

namespace ebiten {
namespace graphics {
namespace detail {

class opengl_initializer : private noncopyable {
private:
  native_view native_view_;
public:
  opengl_initializer(native_view native_view)
    : native_view_(native_view) {
  }
  void
  initialize(std::function<bool()> const& updating_func) {
    [this->native_view_ setUpdatingFunc:updating_func];
  }
};

}
}
}

#endif
