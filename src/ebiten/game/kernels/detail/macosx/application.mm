#include "ebiten/game/kernels/detail/macosx/application.hpp"
#import "ebiten/game/kernels/detail/macosx/application.m"
#include <cassert>

namespace ebiten {
namespace game {
namespace kernels {
namespace detail {

int
do_run(NSWindow* window) {
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
do_run(util::id_ const& native_frame) {
  return do_run(native_frame.get<NSWindow*>());
}

}
}
}
}
