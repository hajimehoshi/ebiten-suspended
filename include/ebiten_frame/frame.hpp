#ifndef EBITEN_FRAME_FRAME_HPP
#define EBITEN_FRAME_FRAME_HPP

#include "ebiten/platform.hpp"

#ifdef EBITEN_MACOSX
# include "ebiten_frame/detail/macosx/frame.hpp"
#endif

#include "ebiten_frame/native_frame.hpp"
#include "ebiten/graphics/native_view.hpp"
#include "ebiten/noncopyable.hpp"

namespace ebiten_frame {

class frame : private ebiten::noncopyable {
private:
  std::size_t const width_;
  std::size_t const height_;
  native_frame const native_frame_;
public:
  frame(std::size_t width,
        std::size_t height)
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
  native_frame
  native_frame() {
    return this->native_frame_;
  }
  ebiten::graphics::native_view
  native_view() {
    return detail::get_native_view(this->native_frame_);
  }
  void
  close() {
    detail::close(this->native_frame_);
  }
};

}

#endif
