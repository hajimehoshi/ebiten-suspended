#ifndef EBITEN_FRAMES_DETAIL_MACOSX_FRAME_HPP
#define EBITEN_FRAMES_DETAIL_MACOSX_FRAME_HPP

#include "ebiten/frames/detail/macosx/frame.mm"
#include "ebiten/frames/native_frame.hpp"
#include "ebiten/graphics/native_view.hpp"
#include <cstddef>
#include <functional>

namespace ebiten {
namespace frames {
namespace detail {

static native_frame
generate_native_frame(std::size_t width,
                      std::size_t height,
                      std::function<bool()> is_terminated) {
  return ::ebiten_frame_detail_generate_native_frame(width, height, is_terminated);
}

static graphics::native_view
get_native_view(NSWindow* window) {
  return [window contentView];
}

}
}
}

#endif
