#include "ebiten/game/frames/cocoa/frame.hpp"
#import "ebiten/game/frames/cocoa/frame.m"
#include <cassert>

namespace ebiten {
namespace game {
namespace frames {
namespace cocoa {

frame::frame(std::size_t width, std::size_t height)
  : window_(0) {
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  EbitenWindow* window = [[EbitenWindow alloc]
                          initWithSize:NSMakeSize(width, height)];
  [pool release];
  this->window_ = static_cast<void*>(window);
}

std::ptrdiff_t
frame::native_frame() const {
  return reinterpret_cast<std::ptrdiff_t>(this->window_);
}

}
}
}
}
