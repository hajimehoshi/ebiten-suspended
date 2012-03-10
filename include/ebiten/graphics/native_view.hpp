#ifndef EBITEN_GRAPHICS_GRAPHICS_NATIVE_VIEW_HPP
#define EBITEN_GRAPHICS_GRAPHICS_NATIVE_VIEW_HPP

#include "ebiten/platform.hpp"

#if defined(EBITEN_WX) || defined(EBITEN_MACOSX) || defined(EBITEN_IOS)
# include "ebiten/graphics/detail/opengl/native_view.hpp"
#endif

namespace ebiten {
namespace graphics {

typedef detail::native_view native_view;

}
}

#endif
