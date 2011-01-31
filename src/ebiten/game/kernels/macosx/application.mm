#include "ebiten/game/kernels/macosx/application.hpp"
#import "ebiten/game/kernels/macosx/application.m"

namespace ebiten {
namespace game {
namespace kernels {
namespace macosx {

int
application::run(const std::ptrdiff_t window_) {
  assert(window_);
  NSWindow* window = reinterpret_cast<NSWindow*>(window_);
  assert(window != nil);
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  EbitenController* controller = [[EbitenController alloc] initWithWindow:window];
  NSApplication* app = [NSApplication sharedApplication];
  [app setDelegate:controller];
  [app run];
  [pool release];
  return 0;
}

}
}
}
}
