#include "ebiten/game/graphics/detail/macosx/view.hpp"
#import "ebiten/game/graphics/detail/macosx/view.m"
#include <boost/function.hpp>
#include <cassert>

namespace ebiten {
namespace game {
namespace graphics {
namespace detail {

void
initialize_view(NSWindow* window,
                std::size_t width,
                std::size_t height,
                boost::function<void()>& update_device) {
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
  NSOpenGLPixelFormat* format = [[[NSOpenGLPixelFormat alloc]
                                  initWithAttributes:attributes]
                                 autorelease];
  EbitenOpenGLView* glView = [[EbitenOpenGLView alloc]
                              initWithFrame:rect
                              pixelFormat:format
                              updateDevice: update_device];
  [window setContentView:glView];
  [pool release];
}

void
initialize_view(util::id_ const& native_frame,
                std::size_t width,
                std::size_t height,
                boost::function<void()>& update_device) {
  initialize_view(native_frame.get<NSWindow*>(), width, height, update_device);
}

}
}
}
}
