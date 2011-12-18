#ifndef EBITEN_FRAMES_FRAME_HPP
#define EBITEN_FRAMES_FRAME_HPP

#include "ebiten/platform.hpp"

#ifdef EBITEN_MACOSX
#include "ebiten/frames/detail/macosx/frame.hpp"
#endif

#include "ebiten/graphics/view.hpp"
#include "ebiten/noncopyable.hpp"

namespace ebiten {
namespace frames {

typedef detail::native_frame native_frame;

class frame : private noncopyable {
private:
  std::size_t const width_;
  std::size_t const height_;
  native_frame const native_frame_;
  graphics::view view_;
public:
  frame(std::size_t width, std::size_t height)
    : width_(width),
      height_(height),
      native_frame_(detail::generate_native_frame(width, height)),
      view_(detail::get_native_view(this->native_frame_)) {
  }
  std::size_t
  width() const {
    return this->width_;
  }
  std::size_t
  height() const {
    return this->height_;
  }
  native_frame
  native_frame() {
    return this->native_frame_;
  }
  graphics::view&
  view() {
    return this->view_;
  }
  graphics::view const&
  view() const {
    return this->view_;
  }
};

}
}

#endif
