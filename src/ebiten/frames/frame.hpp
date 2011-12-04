#ifndef EBITEN_FRAMES_FRAME_HPP
#define EBITEN_FRAMES_FRAME_HPP

#include "ebiten/platform.hpp"

#ifdef EBITEN_MACOSX
#include "ebiten/frames/detail/macosx/frame.hpp"
#endif

#include "ebiten/noncopyable.hpp"

namespace ebiten {
namespace frames {

typedef detail::native_frame_type native_frame_type;

class frame : private noncopyable {
private:
  std::size_t const width_;
  std::size_t const height_;
  native_frame_type native_frame_;
public:
  frame(std::size_t width, std::size_t height)
    : width_(width),
      height_(height),
      native_frame_(detail::generate_native_frame(width, height)) {
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
    return this->native_frame_;
  }
};

}
}

#endif
