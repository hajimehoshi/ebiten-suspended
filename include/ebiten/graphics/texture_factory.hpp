#ifndef EBITEN_GRAPHICS_TEXTURE_FACTORY_HPP
#define EBITEN_GRAPHICS_TEXTURE_FACTORY_HPP

#include "ebiten/platform.hpp"

#ifdef EBITEN_OPENGL
# include "ebiten/graphics/detail/opengl/texture_factory.hpp"
#endif

namespace ebiten {
namespace graphics {

typedef detail::texture_factory texture_factory;

}
}

#endif
