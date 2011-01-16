#include "ebiten/game/video/opengl/cocoa.hpp"
#include <Cocoa/Cocoa.h>

namespace ebiten {
namespace game {
namespace video {
namespace opengl {
namespace cocoa {

bool
initialize() {
  NSOpenGLContext* context = [NSOpenGLContext currentContext];
  if (context == nil) {
    return false;
  }
  const int swap_interval = 1;
  [context setValues:&swap_interval forParameter:NSOpenGLCPSwapInterval];
  return true;
}

}
}
}
}
}
