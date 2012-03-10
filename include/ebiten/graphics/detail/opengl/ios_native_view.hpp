#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_IOS_NATIVE_VIEW_HPP
#define EBITEN_GRAPHICS_DETAIL_OPENGL_IOS_NATIVE_VIEW_HPP

#include "ebiten/input.hpp"

namespace ebiten {
namespace graphics {
namespace detail {

typedef GLKView* native_view;

namespace {

void
native_view_set_input(GLKView* native_view, class input& input) {
  // TODO: implement
  EbitenGLKViewDelegate* delegate = native_view.delegate;
  [delegate setInput:input];
}

}
}
}
}

#endif
