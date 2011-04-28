#ifndef EBITEN_GAME_FRAMES_FRAME_HPP
#define EBITEN_GAME_FRAMES_FRAME_HPP

#include "ebiten/platform.hpp"

#ifdef EBITEN_MACOSX
#include "ebiten/game/frames/detail/macosx/frame.hpp"
#endif

namespace ebiten {
namespace game {
namespace frames {

typedef detail::frame frame;

}
}
}

#endif
