#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_MACOSX_NATIVE_VIEW_HPP
#define EBITEN_GRAPHICS_DETAIL_OPENGL_MACOSX_NATIVE_VIEW_HPP

#include "ebiten/input.hpp"

namespace ebiten {
namespace graphics {
namespace detail {
namespace {

void
native_view_set_input(EbitenOpenGLView* v, class input& input) {
  [v setInput:input];
}

}
}
}
}

#endif
