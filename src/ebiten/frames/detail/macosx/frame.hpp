#ifndef EBITEN_FRAMES_DETAIL_MACOSX_FRAME_HPP
#define EBITEN_FRAMES_DETAIL_MACOSX_FRAME_HPP

#import "ebiten/frames/detail/macosx/frame.m"
#import "ebiten/frames/detail/macosx/view.m"

#include <boost/noncopyable.hpp>
#include <cstddef>
#include <functional>

namespace ebiten {
namespace frames {
namespace detail {

class frame : private boost::noncopyable {
public:
  typedef NSWindow* native_frame_type;
private:
  std::size_t const width_;
  std::size_t const height_;
  NSWindow* native_window_;
public:
  frame(std::size_t width, std::size_t height)
    : width_(width),
      height_(height) {
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    EbitenWindow* window = [[EbitenWindow alloc]
                            initWithSize:NSMakeSize(width, height)];
    this->native_window_ = window;
    assert(window != nil);
    NSRect rect = NSMakeRect(0, 0, this->width_, this->height_);
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
    //[window makeFirstResponder:glView];
    [pool release];
  }
  native_frame_type const&
  native_frame() const {
    return this->native_window_;
  }
  std::size_t
  width() const {
    return this->width_;
  }
  std::size_t
  height() const {
    return this->height_;
  }
  void
  connect_updating(std::function<void()> const& func) {
    NSWindow* window = this->native_window_;
    EbitenOpenGLView* glView = [window contentView];
    [glView connectUpdating:func];
  }
};

}
}
}

#endif