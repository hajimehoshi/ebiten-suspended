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
}

- (CVReturn)getFrameForTime:(CVTimeStamp const*)outputTime;
- (void)updateFrame;
- (void)setUpdatingFunc:(std::function<bool()> const&)updatingFunc;
- (BOOL)acceptsFirstResponder;
- (BOOL)becomeFirstResponder;
- (void)mouseDown:(NSEvent*)theEvent;
- (void)keyDown:(NSEvent*)theEvent;

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
  [self performSelectorOnMainThread:@selector(updateFrame)
                         withObject:nil
                      waitUntilDone:NO];
  return kCVReturnSuccess;
}

- (void)updateFrame {
  if (!self->updatingFunc_) {
    return;
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
    return;
  }
}

- (void)setUpdatingFunc:(std::function<bool()> const&)updatingFunc {
  self->updatingFunc_ = updatingFunc;
}

- (BOOL)acceptsFirstResponder {
  return YES;
}

- (BOOL)becomeFirstResponder {
  return YES;
}

- (void)mouseDown:(NSEvent*)theEvent {
  NSPoint location = [self convertPoint:[theEvent locationInWindow] fromView:nil];
  NSLog(@"%@", NSStringFromPoint(location));
}

- (void)keyDown:(NSEvent*)theEvent {
  NSString* chars = [theEvent characters];
  NSLog(@"%@", chars);
}

@end
#endif

#endif
