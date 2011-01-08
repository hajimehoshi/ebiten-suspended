#include <Cocoa/Cocoa.h>
#include "ebiten/game/opengl/cocoa.h"

bool
ebiten_game_opengl_cocoa_initialize(void) {
  NSOpenGLContext* context = [NSOpenGLContext currentContext];
  if (context == nil) {
    return false;
  }
  const int swap_interval = 1;
  [context setValues:&swap_interval forParameter:NSOpenGLCPSwapInterval];
  return true;
}
