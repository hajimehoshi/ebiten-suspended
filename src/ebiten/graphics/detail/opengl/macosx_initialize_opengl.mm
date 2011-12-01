#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_MACOSX_INITIALIZE_OPENGL_MM
#define EBITEN_GRAPHICS_DETAIL_OPENGL_MACOSX_INITIALIZE_OPENGL_MM

/*
 * Reference:
 *   http://developer.apple.com/library/mac/#qa/qa1385/_index.html
 *   http://www.alecjacobson.com/weblog/?p=2185
 */

#import <Cocoa/Cocoa.h>
#import <QuartzCore/QuartzCore.h>

#include "ebiten/frames/frame.hpp"
#include <functional>

@interface EbitenOpenGLView : NSOpenGLView {
@private
  CVDisplayLinkRef displayLink_;
  std::function<void()> updatingFunc_;
}

- (id)initWithFrame:(NSRect)frame
        pixelFormat:(NSOpenGLPixelFormat*)format
       updatingFunc:(std::function<void()> const&)updatingFunc;
- (CVReturn)getFrameForTime:(CVTimeStamp const*)outputTime;
- (BOOL)acceptsFirstResponder;
- (BOOL)becomeFirstResponder;
- (void)mouseDown:(NSEvent*)theEvent;
- (void)keyDown:(NSEvent*)theEvent;

@end

@implementation EbitenOpenGLView

- (id)initWithFrame:(NSRect)frame
        pixelFormat:(NSOpenGLPixelFormat*)format
       updatingFunc:(std::function<void()> const&)updatingFunc {
  self = [super initWithFrame:frame pixelFormat:format];
  if (self != nil) {
    [self prepareOpenGL];
    self->updatingFunc_ = updatingFunc;
  }
  return self;
}

static CVReturn
EbitenDisplayLinkCallback(CVDisplayLinkRef displayLink,
                          CVTimeStamp const* now,
                          CVTimeStamp const* outputTime,
                          CVOptionFlags flagsIn,
                          CVOptionFlags* flagsOut,
                          void* displayLinkContext)
{
  (void)displayLink;
  (void)now;
  (void)flagsIn;
  (void)flagsOut;
  EbitenOpenGLView* view = (__bridge EbitenOpenGLView*)displayLinkContext;
  CVReturn const result = [view getFrameForTime:outputTime];
  return result;
}

- (void)prepareOpenGL
{
  NSOpenGLContext* openGLContext = [self openGLContext];
  assert(openGLContext != nil);
  GLint const swapInterval = 1;
  [openGLContext setValues:&swapInterval
              forParameter:NSOpenGLCPSwapInterval]; 
  CVDisplayLinkCreateWithActiveCGDisplays(&displayLink_);
  CVDisplayLinkSetOutputCallback(displayLink_,
                                 &EbitenDisplayLinkCallback,
                                 (__bridge void*)self);
  CGLContextObj cglContext = (CGLContextObj)[openGLContext CGLContextObj];
  CGLPixelFormatObj cglPixelFormat = (CGLPixelFormatObj)[[self pixelFormat] CGLPixelFormatObj];
  CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink_, cglContext, cglPixelFormat);
  CVDisplayLinkStart(displayLink_);
}

- (CVReturn)getFrameForTime:(CVTimeStamp const*)outputTime
{
  (void)outputTime;
  NSOpenGLContext* context = [self openGLContext];
  assert(context != nil);
  [context makeCurrentContext];
  CGLLockContext((CGLContextObj)[context CGLContextObj]);
  self->updatingFunc_();
  [context flushBuffer];
  CGLUnlockContext((CGLContextObj)[context CGLContextObj]);
  return kCVReturnSuccess;
}

- (void)dealloc
{
  CVDisplayLinkRelease(displayLink_);
  // Do not call [super dealloc] because of ARC.
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

void
ebiten_graphics_detail_initialize_opengl(ebiten::frames::frame& frame,
                                         std::function<void()> const updating_func) {
  NSRect const rect = NSMakeRect(0, 0, frame.width(), frame.height());
  NSOpenGLPixelFormatAttribute const attributes[] = {
    NSOpenGLPFAWindow,
    NSOpenGLPFADoubleBuffer,
    NSOpenGLPFAAccelerated,
    NSOpenGLPFADepthSize, 32,
    nil,
  };
  NSOpenGLPixelFormat* format = [[NSOpenGLPixelFormat alloc]
                                  initWithAttributes:attributes];
  EbitenOpenGLView* glView = [[EbitenOpenGLView alloc]
                               initWithFrame:rect
                                 pixelFormat:format
                                updatingFunc:updating_func];
  [frame.native_frame() setContentView:glView];
  //[window makeFirstResponder:glView];
}

#endif
