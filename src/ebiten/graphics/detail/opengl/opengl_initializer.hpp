#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_OPENGL_INITIALIZER_HPP
#define EBITEN_GRAPHICS_DETAIL_OPENGL_OPENGL_INITIALIZER_HPP

#ifdef EBITEN_MACOSX
#include "ebiten/graphics/detail/opengl/macosx_opengl_initializer.hpp"
#endif

namespace ebiten {
namespace graphics {
namespace detail {

typedef macosx_opengl_initializer opengl_initializer;

}
}
}

#endif
