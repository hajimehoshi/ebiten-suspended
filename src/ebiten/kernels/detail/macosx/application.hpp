#ifndef EBITEN_KERNELS_DETAIL_MACOSX_APPLICATION_HPP
#define EBITEN_KERNELS_DETAIL_MACOSX_APPLICATION_HPP

#include "ebiten/frames/native_frame.hpp"
#include "ebiten/kernels/detail/macosx/application.mm"
#include "ebiten/noncopyable.hpp"

namespace ebiten {
namespace kernels {
namespace detail {

class application : private noncopyable {
public:
  static void
  run(frames::native_frame native_frame) {
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
};

}
}
}

#endif
