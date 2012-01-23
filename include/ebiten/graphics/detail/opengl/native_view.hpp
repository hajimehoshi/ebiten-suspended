#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_NATIVE_VIEW_MM
#define EBITEN_GRAPHICS_DETAIL_OPENGL_NATIVE_VIEW_MM

#include "ebiten/platform.hpp"

#ifdef EBITEN_MACOSX
# include "ebiten/graphics/detail/opengl/macosx_native_view.mm"
# include "ebiten/graphics/detail/opengl/macosx_native_view.hpp"
#endif

#ifdef EBITEN_IOS
# include "ebiten/graphics/detail/opengl/ios_native_view.mm"
# include "ebiten/graphics/detail/opengl/ios_native_view.hpp"
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
