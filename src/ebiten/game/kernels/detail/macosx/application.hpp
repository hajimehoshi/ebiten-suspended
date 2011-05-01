#ifndef EBITEN_GAME_KERNELS_DETAIL_MACOSX_APPLICATION_HPP
#define EBITEN_GAME_KERNELS_DETAIL_MACOSX_APPLICATION_HPP

#import "ebiten/game/kernels/detail/macosx/application.m"

#include "ebiten/game/frames/frame.hpp"
#include <boost/noncopyable.hpp>

namespace ebiten {
namespace game {
namespace kernels {
namespace detail {

class application : private boost::noncopyable {
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
}

#endif
