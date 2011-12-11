#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_MACOSX_OPENGL_INITIALIZER_HPP
#define EBITEN_GRAPHICS_DETAIL_OPENGL_MACOSX_OPENGL_INITIALIZER_HPP

#include "ebiten/graphics/detail/opengl/macosx_opengl_initializer.mm"

namespace ebiten {
namespace graphics {
namespace detail {

class macosx_opengl_initializer {
public:
  macosx_opengl_initializer() = delete;
  macosx_opengl_initializer(macosx_opengl_initializer const&) = delete;
  macosx_opengl_initializer& operator=(macosx_opengl_initializer const&) = delete;
  static void
  initialize(frames::frame& frame,
             std::function<void()> const updating_func) {
    ::ebiten_graphics_detail_initialize_opengl(frame, updating_func);
  }
  template<class View>
  static void
  initialize_with_view(View& view,
                       std::function<void()> const updating_func) {
    ::ebiten_graphics_detail_initialize_opengl_with_view(view, updating_func);
  }
};

}
}
}

#endif
