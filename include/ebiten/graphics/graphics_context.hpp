#ifndef EBITEN_GRAPHICS_GRAPHICS_CONTEXT_HPP
#define EBITEN_GRAPHICS_GRAPHICS_CONTEXT_HPP

#include "ebiten/platform.hpp"

#if defined(EBITEN_MACOSX) || defined(EBITEN_IOS)
# include "ebiten/graphics/detail/opengl/graphics_context.hpp"
#endif

namespace ebiten {
namespace graphics {

typedef detail::graphics_context graphics_context;

}
}

#endif
