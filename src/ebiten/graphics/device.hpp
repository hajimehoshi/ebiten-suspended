#ifndef EBITEN_GRAPHICS_DEVICE_HPP
#define EBITEN_GRAPHICS_DEVICE_HPP

#include "ebiten/platform.hpp"

#ifdef EBITEN_MACOSX
#include "ebiten/graphics/detail/opengl/device.hpp"
#endif

namespace ebiten {
namespace graphics {

typedef detail::device           device;
typedef detail::graphics_context graphics_context;
typedef detail::texture_factory  texture_factory;

}
}

#endif
