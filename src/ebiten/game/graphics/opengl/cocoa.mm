#include "ebiten/game/graphics/opengl/cocoa.hpp"
#include <Cocoa/Cocoa.h>

namespace ebiten {
namespace game {
namespace graphics {
namespace opengl {

bool
cocoa::initialize() {
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
