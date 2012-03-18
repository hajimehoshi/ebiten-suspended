#ifndef EBITEN_GRAPHICS_TEXTURE_HPP
#define EBITEN_GRAPHICS_TEXTURE_HPP

#include "ebiten/platform.hpp"

#ifdef EBITEN_OPENGL
# include "ebiten/graphics/detail/opengl/texture.hpp"
#endif

namespace ebiten {
namespace graphics {

typedef detail::texture texture;

}
}

#endif
