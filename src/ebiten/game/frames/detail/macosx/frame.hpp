#ifndef EBITEN_GAME_FRAMES_DETAIL_MACOSX_FRAME_HPP
#define EBITEN_GAME_FRAMES_DETAIL_MACOSX_FRAME_HPP

#import "ebiten/game/frames/detail/macosx/frame.m"

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <cstddef>

namespace ebiten {
namespace game {
namespace frames {
namespace detail {

class frame : private boost::noncopyable {
public:
  typedef NSWindow* native_frame_type;
private:
  std::size_t const width_;
  std::size_t const height_;
  NSWindow* native_window_;
public:
  frame(std::size_t width, std::size_t height)
    : width_(width),
      height_(height) {
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    EbitenWindow* window = [[EbitenWindow alloc]
                            initWithSize:NSMakeSize(width, height)];
    [pool release];
    this->native_window_ = window;
  }
  ~frame() {
  }
  native_frame_type const&
  native_frame() const {
    return this->native_window_;
  }
  std::size_t
  width() const {
    return this->width_;
  }
  std::size_t
  height() const {
    return this->height_;
  }
};

}
}
}
}

#endif
