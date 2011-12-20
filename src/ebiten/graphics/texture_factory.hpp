#ifndef EBITEN_GRAPHICS_TEXTURE_FACTORY_HPP
#define EBITEN_GRAPHICS_TEXTURE_FACTORY_HPP

#include "ebiten/platform.hpp"

#if defined(EBITEN_MACOSX) || defined(EBITEN_IOS)
# include "ebiten/graphics/detail/opengl/texture_factory.hpp"
#endif

namespace ebiten {
namespace graphics {

typedef detail::texture_factory texture_factory;

}
}

#endif
