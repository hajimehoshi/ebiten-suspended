#include "ebiten/game/kernels/macosx/application.hpp"
#import "ebiten/game/kernels/macosx/application.m"
#include <cassert>

namespace ebiten {
namespace game {
namespace kernels {
namespace macosx {

namespace detail {

int
run(NSWindow* window) {
  assert(window != nil);
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  EbitenController* controller = [[EbitenController alloc] initWithWindow:window];
  NSApplication* app = [NSApplication sharedApplication];
  [app setDelegate:controller];
  [app run];
  [pool release];
  return 0;
}

int
run(const std::ptrdiff_t native_frame) {
  assert(native_frame);
  return run(reinterpret_cast<NSWindow*>(native_frame));
}

}

}
}
}
}
