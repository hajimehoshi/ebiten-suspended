#include "ebiten/game/graphics/opengl/cocoa/view.hpp"
#import "ebiten/game/graphics/opengl/cocoa/view.m"
#include <cassert>

namespace ebiten {
namespace game {
namespace graphics {
namespace opengl {
namespace cocoa {

view::view(std::ptrdiff_t native_frame)
  : gl_view_(0) {
  assert(native_frame);
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  NSRect rect = NSMakeRect(0, 0, 640, 480);
  NSOpenGLPixelFormatAttribute attributes[] = {
    NSOpenGLPFAWindow,
    NSOpenGLPFADoubleBuffer,
    NSOpenGLPFAAccelerated,
    NSOpenGLPFADepthSize, 16,
    nil,
  };
  NSOpenGLPixelFormat* format = [[[NSOpenGLPixelFormat alloc] initWithAttributes:attributes]
                                 autorelease];
  //NSOpenGLContext* context = [[NSOpenGLContext alloc] initWithFormat:format shareContext:nil];
  NSOpenGLView* glView = [[NSOpenGLView alloc] initWithFrame:rect pixelFormat:format];
  NSWindow* window = reinterpret_cast<NSWindow*>(native_frame);
  [window setContentView:glView];
  [pool release];
  this->gl_view_ = static_cast<void*>(glView);
}

}
}
}
}
}

