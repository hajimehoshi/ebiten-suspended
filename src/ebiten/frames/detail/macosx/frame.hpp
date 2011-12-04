#ifndef EBITEN_FRAMES_DETAIL_MACOSX_FRAME_HPP
#define EBITEN_FRAMES_DETAIL_MACOSX_FRAME_HPP

#include "ebiten/frames/detail/macosx/frame.mm"
#include <cstddef>

namespace ebiten {
namespace frames {
namespace detail {

typedef NSWindow* native_frame_type;

NSWindow*
generate_native_frame(std::size_t width, std::size_t height) {
  return ::ebiten_frame_detail_generate_native_frame(width, height);
}

}
}
}

#endif
