#ifndef EBITEN_GAME_GRAPHICS_DETAIL_MACOSX_COCOA_HPP
#define EBITEN_GAME_GRAPHICS_DETAIL_MACOSX_COCOA_HPP

#import "ebiten/game/graphics/detail/macosx/view.m"

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

namespace ebiten {
namespace game {
namespace graphics {
namespace detail {

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
    NSWindow* window = this->frame_.native_frame();
    assert(window != nil);
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    NSRect rect = NSMakeRect(0, 0, frame_width, frame_height);
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
                                pixelFormat:format];
    [window setContentView:glView];
    [glView connectUpdatingDevice:update_device];
    [pool release];
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
