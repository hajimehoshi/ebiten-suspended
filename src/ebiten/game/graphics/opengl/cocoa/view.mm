#include "ebiten/game/graphics/opengl/cocoa/view.hpp"
#import "ebiten/game/graphics/opengl/cocoa/view.m"
#include <boost/function.hpp>
#include <cassert>

namespace ebiten {
namespace game {
namespace graphics {
namespace opengl {
namespace cocoa {

namespace detail {

void
initialize(NSWindow* window, boost::function<void()> update_device) {
  assert(window != nil);
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  NSRect rect = NSMakeRect(0, 0, 640, 480);
  NSOpenGLPixelFormatAttribute attributes[] = {
    NSOpenGLPFAWindow,
    NSOpenGLPFADoubleBuffer,
    NSOpenGLPFAAccelerated,
    NSOpenGLPFADepthSize, 32,
    nil,
  };
  NSOpenGLPixelFormat* format = [[[NSOpenGLPixelFormat alloc] initWithAttributes:attributes]
                                 autorelease];
  //NSOpenGLContext* context = [[NSOpenGLContext alloc] initWithFormat:format shareContext:nil];
  EbitenOpenGLView* glView = [[EbitenOpenGLView alloc]
                              initWithFrame:rect
                              pixelFormat:format
                              updateDevice: update_device];
  [window setContentView:glView];
  [pool release];
}

void
initialize(const util::id_& native_frame, boost::function<void()> update_device) {
  initialize(native_frame.get<NSWindow*>(), update_device);
}

}

}
}
}
}
}
