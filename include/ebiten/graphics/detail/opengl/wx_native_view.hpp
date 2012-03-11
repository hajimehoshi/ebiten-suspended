#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_WX_NATIVE_VIEW_HPP
#define EBITEN_GRAPHICS_DETAIL_OPENGL_WX_NATIVE_VIEW_HPP

#include "ebiten/input.hpp"
#include <wx/window.h>

namespace ebiten {
namespace graphics {
namespace detail {

typedef wxGLCanvas* native_view;

namespace {
void
native_view_set_input(native_view nv, class input& input) {
  //EbitenGLKViewDelegate* delegate = native_view.delegate;
  //[delegate setInput:input];
}

}

}
}
}

#endif
