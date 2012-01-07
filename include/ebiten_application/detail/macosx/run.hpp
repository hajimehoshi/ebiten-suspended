#ifndef EBITEN_APPLICATION_DETAIL_MACOSX_RUN_HPP
#define EBITEN_APPLICATION_DETAIL_MACOSX_RUN_HPP

#include "ebiten_application/detail/macosx/run.mm"
#include "ebiten/frames/native_frame.hpp"

namespace ebiten_application {
namespace detail {
namespace {

void
run(ebiten::frames::native_frame native_frame) {
  @autoreleasepool {
    NSWindow* window = native_frame;
    assert(window != nil);
    EbitenController* controller = [[EbitenController alloc] initWithWindow:window];
    NSApplication* app = [NSApplication sharedApplication];
    [app setDelegate:controller];
    [app finishLaunching];
    [app run];
  }
}

}
}
}

#endif
