#ifndef EBITEN_GRAPHICS_TEXTURE_POINTER_HPP
#define EBITEN_GRAPHICS_TEXTURE_POINTER_HPP

#include "ebiten/platform.hpp"

#ifdef EBITEN_OPENGL
# include "ebiten/graphics/detail/opengl/texture_pointer.hpp"
#endif

namespace ebiten {
namespace graphics {

typedef detail::texture_pointer texture_pointer;

}
}

#endif
