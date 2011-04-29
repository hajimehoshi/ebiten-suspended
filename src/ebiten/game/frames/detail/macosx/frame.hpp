#ifndef EBITEN_GAME_FRAMES_DETAIL_MACOSX_FRAME_HPP
#define EBITEN_GAME_FRAMES_DETAIL_MACOSX_FRAME_HPP

#import "ebiten/game/frames/detail/macosx/frame.m"

#include "ebiten/util/id.hpp"
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <cstddef>

namespace ebiten {
namespace game {
namespace frames {
namespace detail {

class frame : private boost::noncopyable {
private:
  std::size_t const width_;
  std::size_t const height_;
  util::id_ window_;
public:
  frame(std::size_t width, std::size_t height)
    : width_(width),
      height_(height) {
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    EbitenWindow* window = [[EbitenWindow alloc]
                            initWithSize:NSMakeSize(width, height)];
    [pool release];
    this->window_ = util::id_(window);
  }
  ~frame() {
  }
  util::id_ const&
  native_frame() const {
    return this->window_;
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
