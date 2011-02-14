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
initialize(NSWindow* window,
           std::size_t width,
           std::size_t height,
           boost::function<void()> update_device) {
  assert(window != nil);
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  NSRect rect = NSMakeRect(0, 0, width, height);
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
initialize(util::id_ const& native_frame,
           std::size_t width,
           std::size_t height,
           boost::function<void()> update_device) {
  initialize(native_frame.get<NSWindow*>(), width, height, update_device);
}

}

}
}
}
}
}
