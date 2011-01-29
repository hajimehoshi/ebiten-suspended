#include "ebiten/game/frames/cocoa/frame.hpp"
#import <Cocoa/Cocoa.h>

@interface Hoge : NSObject
{
}
@end

namespace ebiten {
namespace game {
namespace frames {
namespace cocoa {

int
frame::run() {
  id app = [NSApplication sharedApplication];
  [app run];
  return 0;
}

}
}
}
}
