#ifndef EBITEN_KERNEL_DETAIL_MACOSX_APPLICATION_HPP
#define EBITEN_KERNEL_DETAIL_MACOSX_APPLICATION_HPP

#import "ebiten/kernel/detail/macosx/application.mm"

#include "ebiten/frames/frame.hpp"

namespace ebiten {
namespace kernel {
namespace detail {

void
run_application(frames::frame& frame) {
  NSWindow* window = frame.native_frame();
  assert(window != nil);
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  EbitenController* controller = [[EbitenController alloc] initWithWindow:window];
  NSApplication* app = [NSApplication sharedApplication];
  [app setDelegate:controller];
  [app finishLaunching];
  [app run];
  [pool release];
}

}
}
}

#endif
