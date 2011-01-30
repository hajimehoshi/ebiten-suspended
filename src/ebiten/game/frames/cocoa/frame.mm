#include "ebiten/game/frames/cocoa/frame.hpp"
#import "ebiten/game/frames/cocoa/frame.m"

namespace ebiten {
namespace game {
namespace frames {
namespace cocoa {

frame::frame(std::size_t width, std::size_t height) {
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  EbitenController* controller = [[EbitenController alloc] init];
  NSApplication* app = [NSApplication sharedApplication];
  [app setDelegate:controller];
  [pool release];
}

int
frame::run() {
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  NSApplication* app = [NSApplication sharedApplication];
  [app run];
  [pool release];
  return 0;
}

}
}
}
}
