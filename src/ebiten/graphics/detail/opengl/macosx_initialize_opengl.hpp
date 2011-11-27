#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_MACOSX_INITIALIZE_OPENGL_HPP
#define EBITEN_GRAPHICS_DETAIL_OPENGL_MACOSX_INITIALIZE_OPENGL_HPP

#import "ebiten/graphics/detail/opengl/macosx_view.m"

#include <functional>

namespace ebiten {
namespace graphics {
namespace detail {

void
initialize_opengl(frames::frame& frame,
                  std::function<void()> const updating_func) {
  NSRect rect = NSMakeRect(0, 0, frame.width(), frame.height());
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
                              updatingFunc:updating_func];
  [frame.native_frame() setContentView:glView];
  //[window makeFirstResponder:glView];
}

}
}
}

#endif
