#ifndef EBITEN_FRAME_DETAIL_MACOSX_FRAME_HPP
#define EBITEN_FRAME_DETAIL_MACOSX_FRAME_HPP

#include "ebiten_frame/detail/macosx/frame.mm"
#include "ebiten_frame/native_frame.hpp"
#include "ebiten/graphics/native_view.hpp"
#include <cstddef>

namespace ebiten_frame {
namespace detail {

static native_frame
generate_native_frame(std::size_t width,
                      std::size_t height) {
  return ::ebiten_frame_detail_generate_native_frame(width, height);
}

static ebiten::graphics::native_view
get_native_view(NSWindow* window) {
  return [window contentView];
}

static void
close(NSWindow* window) {
  [window close];
}

}
}

#endif
