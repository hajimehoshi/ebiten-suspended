#include "ebiten/game/frames/cocoa/frame.hpp"
#import "ebiten/game/frames/cocoa/frame.m"

namespace ebiten {
namespace game {
namespace frames {
namespace cocoa {

frame::frame(std::size_t width, std::size_t height)
  : controller_(0) {
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  EbitenController* controller = [[EbitenController alloc]
                                  initWithWidth:width height:height];
  [[NSApplication sharedApplication] setDelegate:controller];
  [pool release];
  this->controller_ = static_cast<void*>(controller);
}

std::ptrdiff_t
frame::native_frame() const {
  EbitenController* controller = static_cast<EbitenController*>(this->controller_);
  NSWindow* window = [controller window];
  return reinterpret_cast<std::ptrdiff_t>(window);
}

}
}
}
}
