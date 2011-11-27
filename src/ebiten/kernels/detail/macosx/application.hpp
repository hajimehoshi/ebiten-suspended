#ifndef EBITEN_KERNELS_DETAIL_MACOSX_APPLICATION_HPP
#define EBITEN_KERNELS_DETAIL_MACOSX_APPLICATION_HPP

#import "ebiten/kernels/detail/macosx/application.m"

#include "ebiten/frames/frame.hpp"
#include "ebiten/util/noncopyable.hpp"

namespace ebiten {
namespace kernels {
namespace detail {

class application : private ebiten::util::noncopyable {
public:
  explicit
  application(frames::frame& frame) {
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
};

}
}
}

#endif
