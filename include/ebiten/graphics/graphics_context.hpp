#ifndef EBITEN_GRAPHICS_GRAPHICS_CONTEXT_HPP
#define EBITEN_GRAPHICS_GRAPHICS_CONTEXT_HPP

#include "ebiten/platform.hpp"

#ifdef EBITEN_OPENGL
# include "ebiten/graphics/detail/opengl/graphics_context.hpp"
#endif

namespace ebiten {
namespace graphics {

typedef detail::graphics_context graphics_context;

}
}

#endif
