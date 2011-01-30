#include "ebiten/game/kernels/macosx/application.hpp"
#import "ebiten/game/kernels/macosx/application.m"

namespace ebiten {
namespace game {
namespace kernels {
namespace macosx {

int
application::run() {
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  [[NSApplication sharedApplication] run];
  [pool release];
  return 0;
}

}
}
}
}
