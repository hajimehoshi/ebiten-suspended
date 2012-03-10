#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_IOS_NATIVE_VIEW_HPP
#define EBITEN_GRAPHICS_DETAIL_OPENGL_IOS_NATIVE_VIEW_HPP

#include "ebiten/input.hpp"

namespace ebiten {
namespace graphics {
namespace detail {

typedef GLKView* native_view;

namespace {

void
native_view_set_input(native_view* nv, class input& input) {
  // TODO: implement
  EbitenGLKViewDelegate* delegate = nv.delegate;
  [delegate setInput:input];
}

}
}
}
}

#endif
