#ifndef EBITEN_FRAMES_NATIVE_FRAME_HPP
#define EBITEN_FRAMES_NATIVE_FRAME_HPP

#include "ebiten/platform.hpp"

#ifdef EBITEN_MACOSX
#include "ebiten/frames/detail/macosx/native_frame.hpp"
#endif

namespace ebiten {
namespace frames {

typedef detail::native_frame native_frame;

}
}

#endif
