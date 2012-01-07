#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_NATIVE_VIEW_MM
#define EBITEN_GRAPHICS_DETAIL_OPENGL_NATIVE_VIEW_MM

#include "ebiten/platform.hpp"

#ifdef EBITEN_MACOSX
# include "ebiten/graphics/detail/opengl/macosx_native_view.mm"
#endif

namespace ebiten {
namespace graphics {
namespace detail {

#ifdef EBITEN_MACOSX
typedef EbitenOpenGLView* native_view;
#endif

#ifdef EBITEN_IOS
typedef GLKView* native_view;
#endif

}
}
}

#endif
