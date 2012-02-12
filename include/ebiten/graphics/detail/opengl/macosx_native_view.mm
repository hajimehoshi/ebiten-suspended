#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_MACOSX_NATIVE_VIEW_MM
#define EBITEN_GRAPHICS_DETAIL_OPENGL_MACOSX_NATIVE_VIEW_MM

/*
 * Reference:
 *   http://developer.apple.com/library/mac/#qa/qa1385/_index.html
 *   http://www.alecjacobson.com/weblog/?p=2185
 */

// TODO: Use NSViewController?

#include "ebiten/input.hpp"
#import <Cocoa/Cocoa.h>
#import <QuartzCore/QuartzCore.h>
#include <functional>

@interface EbitenOpenGLView : NSOpenGLView {
@private
  CVDisplayLinkRef displayLink_;
  std::function<bool()> updatingFunc_;
  ebiten::input* input_;
}

- (CVReturn)getFrameForTime:(CVTimeStamp const*)outputTime;
- (void)setUpdatingFunc:(std::function<bool()> const&)updatingFunc;

@end

static CVReturn
EbitenDisplayLinkCallback(CVDisplayLinkRef displayLink,
                          CVTimeStamp const* now,
                          CVTimeStamp const* outputTime,
                          CVOptionFlags flagsIn,
                          CVOptionFlags* flagsOut,
                          void* displayLinkContext) {
  (void)displayLink;
  (void)now;
  (void)flagsIn;
  (void)flagsOut;
  @autoreleasepool {
    EbitenOpenGLView* view = (__bridge EbitenOpenGLView*)displayLinkContext;
    return [view getFrameForTime:outputTime];
  }
}

#ifndef EBITEN_WITHOUT_OBJC_IMPL
@implementation EbitenOpenGLView

- (void)dealloc {
  ::CVDisplayLinkRelease(self->displayLink_);
  // Do not call [super dealloc] because of ARC.
}

- (void)prepareOpenGL {
  [super prepareOpenGL];
  NSOpenGLContext* openGLContext = [self openGLContext];
  assert(openGLContext != nil);
  GLint const swapInterval = 1;
  [openGLContext setValues:&swapInterval
              forParameter:NSOpenGLCPSwapInterval]; 
  ::CVDisplayLinkCreateWithActiveCGDisplays(&self->displayLink_);
  ::CVDisplayLinkSetOutputCallback(self->displayLink_,
                                   &EbitenDisplayLinkCallback,
                                   (__bridge void*)self);
  CGLContextObj cglContext = (CGLContextObj)[openGLContext CGLContextObj];
  CGLPixelFormatObj cglPixelFormat = (CGLPixelFormatObj)[[self pixelFormat] CGLPixelFormatObj];
  ::CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(self->displayLink_,
                                                      cglContext,
                                                      cglPixelFormat);
  ::CVDisplayLinkStart(self->displayLink_);
}

- (CVReturn)getFrameForTime:(CVTimeStamp const*)outputTime {
  (void)outputTime;
  if (!self->updatingFunc_) {
    return kCVReturnSuccess;;
  }
  NSOpenGLContext* context = [self openGLContext];
  assert(context != nil);
  [context makeCurrentContext];
  bool terminated = false;
  {
    ::CGLLockContext((CGLContextObj)[context CGLContextObj]);
    terminated = self->updatingFunc_();
    [context flushBuffer];
    ::CGLUnlockContext((CGLContextObj)[context CGLContextObj]);
  }
  if (terminated) {
    // It may cause problems when closing the window.
    //::CVDisplayLinkStop(self->displayLink_);
    return kCVReturnSuccess;
  }
  return kCVReturnSuccess;
}

- (void)setUpdatingFunc:(std::function<bool()> const&)func {
  self->updatingFunc_ = func;
}

- (void)setInput:(ebiten::input&)input {
  self->input_ = &input;
}

- (BOOL)isFlipped {
  return YES;
}

- (void)mouseDown:(NSEvent*)theEvent {
  NSPoint location = [self convertPoint:[theEvent locationInWindow]
                               fromView:nil];
  if (self->input_) {
    int x = location.x;
    int y = location.y;
    // TODO: Screen size
    self->input_->set_touches_real_location(0,
                                            static_cast<int>(x),
                                            static_cast<int>(y));
    self->input_->set_touched(0, true);
  }
}

- (void)mouseUp:(NSEvent*)theEvent {
  (void)theEvent;
  if (self->input_) {
    self->input_->set_touches_real_location(0, -1, -1);
    self->input_->set_touched(0, false);
  }
}

- (void)mouseDragged:(NSEvent*)theEvent {
  NSPoint location = [self convertPoint:[theEvent locationInWindow]
                               fromView:nil];
  if (self->input_) {
    int x = location.x;
    int y = location.y;
    self->input_->set_touches_real_location(0,
                                            static_cast<int>(x),
                                            static_cast<int>(y));
    self->input_->set_touched(0, true);
  }
}

@end
#endif

#endif
