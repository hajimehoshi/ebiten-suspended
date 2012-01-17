#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_MACOSX_NATIVE_VIEW_MM
#define EBITEN_GRAPHICS_DETAIL_OPENGL_MACOSX_NATIVE_VIEW_MM

/*
 * Reference:
 *   http://developer.apple.com/library/mac/#qa/qa1385/_index.html
 *   http://www.alecjacobson.com/weblog/?p=2185
 */

#import <Cocoa/Cocoa.h>
#import <QuartzCore/QuartzCore.h>
#include <functional>

@interface EbitenOpenGLView : NSOpenGLView {
@private
  CVDisplayLinkRef displayLink_;
  std::function<bool()> updatingFunc_;
  std::function<void(int, int, int)> settingTouchesLocationFunc_;
  std::function<void(int, bool)> settingTouchedFunc_;
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
    ::CVDisplayLinkStop(self->displayLink_);
    return kCVReturnSuccess;;
  }
  return kCVReturnSuccess;
}

- (void)setUpdatingFunc:(std::function<bool()> const&)func {
  self->updatingFunc_ = func;
}

- (void)setSettingTouchesLocationFunc:(std::function<void(int, int, int)> const&)func {
  self->settingTouchesLocationFunc_ = func;
}

- (void)setSettingTouchedFunc:(std::function<void(int, bool)> const&)func {
  self->settingTouchedFunc_ = func;
}

- (void)mouseDown:(NSEvent*)theEvent {
  NSPoint location = [self convertPoint:[theEvent locationInWindow] fromView:nil];
  if (self->settingTouchesLocationFunc_) {
    self->settingTouchesLocationFunc_(0,
                                      static_cast<int>(location.x),
                                      static_cast<int>(location.y));
  }
  if (self->settingTouchedFunc_) {
    self->settingTouchedFunc_(0, true);
  }
}

- (void)mouseUp:(NSEvent*)theEvent {
  (void)theEvent;
  if (self->settingTouchesLocationFunc_) {
    self->settingTouchesLocationFunc_(0, -1, -1);
  }
  if (self->settingTouchedFunc_) {
    self->settingTouchedFunc_(0, false);
  }
}

- (void)mouseDragged:(NSEvent*)theEvent {
  NSPoint location = [self convertPoint:[theEvent locationInWindow] fromView:nil];
  if (self->settingTouchesLocationFunc_) {
    self->settingTouchesLocationFunc_(0,
                                      static_cast<int>(location.x),
                                      static_cast<int>(location.y));
  }
  if (self->settingTouchedFunc_) {
    self->settingTouchedFunc_(0, true);
  }
}

@end
#endif

#endif
