#ifndef EBITEN_FRAMES_FRAME_HPP
#define EBITEN_FRAMES_FRAME_HPP

#include "ebiten/platform.hpp"

#ifdef EBITEN_MACOSX
#include "ebiten/frames/detail/macosx/frame.hpp"
#endif

namespace ebiten {
namespace frames {

typedef detail::frame frame;

}
}

#endif
