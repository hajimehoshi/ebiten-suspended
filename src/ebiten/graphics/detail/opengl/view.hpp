#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_VIEW_MM
#define EBITEN_GRAPHICS_DETAIL_OPENGL_VIEW_MM

#ifdef EBITEN_MACOSX
# include "macosx_view.mm"
#endif

namespace ebiten {
namespace graphics {
namespace detail {

#ifdef EBITEN_MACOSX
typedef EbitenOpenGLView* native_view;
#endif

}
}
}

#endif

