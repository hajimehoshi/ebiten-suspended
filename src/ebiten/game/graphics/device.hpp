#ifndef EBITEN_GAME_GRAPHICS_DEVICE_HPP
#define EBITEN_GAME_GRAPHICS_DEVICE_HPP

#include "ebiten/platform.hpp"

#ifdef EBITEN_MACOSX
#include "ebiten/game/graphics/detail/macosx/device.hpp"
#endif

#include "ebiten/game/frames/frame.hpp"

namespace ebiten {
namespace game {
namespace graphics {

typedef detail::device<frames::frame> device;

}
}
}

#endif
