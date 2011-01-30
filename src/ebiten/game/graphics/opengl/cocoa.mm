#include "ebiten/game/graphics/opengl/cocoa.hpp"
#import "ebiten/game/graphics/opengl/cocoa.m"

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
