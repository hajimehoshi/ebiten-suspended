#ifndef EBITEN_GAME_KERNELS_DETAIL_MACOSX_APPLICATION_HPP
#define EBITEN_GAME_KERNELS_DETAIL_MACOSX_APPLICATION_HPP

#import "ebiten/game/kernels/detail/macosx/application.m"

#include "ebiten/util/id.hpp"
#include <boost/noncopyable.hpp>

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

class application : private boost::noncopyable {
public:
  template<class Device>
  explicit
  application(Device& device) {
    do_run(device.frame().native_frame());
  }
};

}
}
}
}

#endif
