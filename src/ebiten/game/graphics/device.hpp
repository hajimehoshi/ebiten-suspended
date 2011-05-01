#ifndef EBITEN_GAME_GRAPHICS_DEVICE_HPP
#define EBITEN_GAME_GRAPHICS_DEVICE_HPP

#include "ebiten/platform.hpp"

#ifdef EBITEN_MACOSX
#include "ebiten/game/graphics/detail/opengl/device.hpp"
#endif

namespace ebiten {
namespace game {
namespace graphics {

typedef detail::device device;

}
}
}

#endif
