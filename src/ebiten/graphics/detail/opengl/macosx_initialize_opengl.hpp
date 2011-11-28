#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_MACOSX_INITIALIZE_OPENGL_HPP
#define EBITEN_GRAPHICS_DETAIL_OPENGL_MACOSX_INITIALIZE_OPENGL_HPP

#include "ebiten/graphics/detail/opengl/macosx_initialize_opengl.mm"

namespace ebiten {
namespace graphics {
namespace detail {

void
initialize_opengl(frames::frame& frame,
                  std::function<void()> const updating_func) {
  ::ebiten_graphics_detail_initialize_opengl(frame, updating_func);
}

}
}
}

#endif
