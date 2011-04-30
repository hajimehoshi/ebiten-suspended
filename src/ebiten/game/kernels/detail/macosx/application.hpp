#ifndef EBITEN_GAME_KERNELS_DETAIL_MACOSX_APPLICATION_HPP
#define EBITEN_GAME_KERNELS_DETAIL_MACOSX_APPLICATION_HPP

#import "ebiten/game/kernels/detail/macosx/application.m"

#include <boost/noncopyable.hpp>

namespace ebiten {
namespace game {
namespace kernels {
namespace detail {

class application : private boost::noncopyable {
public:
  template<class Device>
  explicit
  application(Device& device) {
    NSWindow* window = device.frame().native_frame();
    assert(window != nil);
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    EbitenController* controller = [[EbitenController alloc] initWithWindow:window];
    NSApplication* app = [NSApplication sharedApplication];
    [app setDelegate:controller];
    {
      // menu
      //[NSBundle loadNibNamed:@"MainMenu" owner:app];
      //NSLog(@"%@", [app mainMenu]);
      /*NSMenu* appleMenu = [[NSMenu alloc] initWithTitle:@"hoge"];
      [app setAppleMenu:appleMenu];
      [appleMenu release];*/
    }
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
