#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_MACOSX_NATIVE_VIEW_HPP
#define EBITEN_GRAPHICS_DETAIL_OPENGL_MACOSX_NATIVE_VIEW_HPP

#include "ebiten/input.hpp"

namespace ebiten {
namespace graphics {
namespace detail {

typedef EbitenOpenGLView* native_view;

namespace {

void
native_view_set_input(native_view nv, class input& input) {
  [nv setInput:input];
}

}
}
}
}

#endif
