#include "ebiten/game/frames/cocoa/frame.hpp"
#import "ebiten/game/frames/cocoa/frame.m"
#include <cassert>

namespace ebiten {
namespace game {
namespace frames {
namespace cocoa {

struct frame::impl {
  EbitenWindow* window_;
};

frame::frame(std::size_t width, std::size_t height)
  : pimpl_(new impl) {
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  EbitenWindow* window = [[EbitenWindow alloc]
                          initWithSize:NSMakeSize(width, height)];
  [pool release];
  this->pimpl_->window_ = window;
}

frame::~frame() {
}

std::ptrdiff_t
frame::native_frame() const {
  return reinterpret_cast<std::ptrdiff_t>(this->pimpl_->window_);;
}

}
}
}
}
