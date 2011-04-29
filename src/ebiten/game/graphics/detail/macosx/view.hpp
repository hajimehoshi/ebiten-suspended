#ifndef EBITEN_GAME_GRAPHICS_DETAIL_MACOSX_COCOA_HPP
#define EBITEN_GAME_GRAPHICS_DETAIL_MACOSX_COCOA_HPP

#import "ebiten/game/graphics/detail/macosx/view.m"

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

namespace ebiten {
namespace game {
namespace graphics {
namespace detail {

void initialize_view(NSWindow* window,
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

// TODO: private constructor?
template<class Frame>
class view : private boost::noncopyable {
private:
  Frame frame_;
public:
  typedef Frame frame_type;
  view(std::size_t frame_width,
       std::size_t frame_height,
       boost::function<void()> update_device)
    : frame_(frame_width, frame_height) {
    initialize_view(frame_.native_frame(), frame_.width(), frame_.height(), update_device);
  }
  Frame&
  frame() {
    return this->frame_;
  }
};

}
}
}
}

#endif
