#ifndef EBITEN_FRAME_NATIVE_FRAME_HPP
#define EBITEN_FRAME_NATIVE_FRAME_HPP

#include "ebiten/platform.hpp"

#ifdef EBITEN_MACOSX
#include "ebiten_frame/detail/macosx/native_frame.hpp"
#endif

namespace ebiten_frame {

typedef detail::native_frame native_frame;

}

#endif
