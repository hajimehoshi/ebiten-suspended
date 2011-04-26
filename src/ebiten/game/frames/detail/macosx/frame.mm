#include "ebiten/game/frames/detail/macosx/frame.hpp"
#import "ebiten/game/frames/detail/macosx/frame.m"
#include <boost/scoped_ptr.hpp>
#include <cassert>

namespace ebiten {
namespace game {
namespace frames {
namespace detail {

frame::frame(std::size_t width, std::size_t height)
  : width_(width),
    height_(height) {
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  EbitenWindow* window = [[EbitenWindow alloc]
                          initWithSize:NSMakeSize(width, height)];
  [pool release];
  this->window_ = util::id_(window);
}

frame::~frame() {
}

util::id_ const&
frame::native_frame() const {
  return this->window_;
}

}
}
}
}
