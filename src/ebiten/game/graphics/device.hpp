#ifndef EBITEN_GAME_GRAPHICS_DEVICE_HPP
#define EBITEN_GAME_GRAPHICS_DEVICE_HPP

#include "ebiten/platform.hpp"

#ifdef EBITEN_MACOSX
#include "ebiten/game/graphics/detail/macosx/device.hpp"
#endif

namespace ebiten {
namespace game {
namespace graphics {

// TODO: change it!
template<class Frame>
struct device {
  typedef detail::device<Frame> type;
};

}
}
}

#endif
