#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_OPENGL_INITIALIZER_HPP
#define EBITEN_GRAPHICS_DETAIL_OPENGL_OPENGL_INITIALIZER_HPP

#ifdef EBITEN_MACOSX
# include "ebiten/graphics/detail/opengl/macosx_opengl_initializer.mm"
#endif

#ifdef EBITEN_IOS
# include "ebiten/graphics/detail/opengl/ios_opengl_initializer.mm"
#endif

namespace ebiten {
namespace graphics {
namespace detail {

class opengl_initializer {
public:
  opengl_initializer() = delete;
  opengl_initializer(opengl_initializer const&) = delete;
  opengl_initializer& operator=(opengl_initializer const&) = delete;
#ifndef EBITEN_IOS
  static void
  initialize(frames::frame& frame,
             std::function<void()> const updating_func) {
    ::ebiten_graphics_detail_initialize_opengl(frame, updating_func);
  }
#endif
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
