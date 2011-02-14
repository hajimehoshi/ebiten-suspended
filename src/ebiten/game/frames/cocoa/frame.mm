#include "ebiten/game/frames/cocoa/frame.hpp"
#import "ebiten/game/frames/cocoa/frame.m"
#include <boost/scoped_ptr.hpp>
#include <cassert>

namespace ebiten {
namespace game {
namespace frames {
namespace cocoa {

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
