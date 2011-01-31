#include "ebiten/game/graphics/opengl/cocoa/view.hpp"
#import "ebiten/game/graphics/opengl/cocoa/view.m"
#include <cassert>

namespace ebiten {
namespace game {
namespace graphics {
namespace opengl {
namespace cocoa {

struct view::impl {
  NSOpenGLView* gl_view_;
};

view::view(const std::ptrdiff_t native_frame)
  : pimpl_(new impl) {
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
  id window = reinterpret_cast<id>(native_frame);
  assert(window != nil);
  [window setContentView:glView];
  [pool release];
  this->pimpl_->gl_view_ = glView;
}

view::~view() {
}

}
}
}
}
}
