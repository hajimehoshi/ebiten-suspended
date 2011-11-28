#ifndef EBITEN_FRAMES_FRAME_HPP
#define EBITEN_FRAMES_FRAME_HPP

#include "ebiten/platform.hpp"

#ifdef EBITEN_MACOSX
#include "ebiten/frames/detail/macosx/frame.hpp"
#endif

namespace ebiten {
namespace frames {

typedef detail::native_frame_type native_frame_type;

class frame : private ebiten::util::noncopyable {
private:
  std::size_t const width_;
  std::size_t const height_;
  native_frame_type native_window_;
public:
  frame(std::size_t width, std::size_t height)
    : width_(width),
      height_(height),
      native_window_(detail::generate_native_window(width, height)) {
  }
  std::size_t
  width() const {
    return this->width_;
  }
  std::size_t
  height() const {
    return this->height_;
  }
  native_frame_type const&
  native_frame() const {
    return this->native_window_;
  }
};

}
}

#endif
