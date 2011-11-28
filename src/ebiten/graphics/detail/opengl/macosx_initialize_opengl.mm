#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_MACOSX_INITIALIZE_OPENGL_MM
#define EBITEN_GRAPHICS_DETAIL_OPENGL_MACOSX_INITIALIZE_OPENGL_MM

#import <Cocoa/Cocoa.h>

#include "ebiten/frames/frame.hpp"
#include <functional>

@interface EbitenOpenGLView : NSOpenGLView {
  std::function<void()> updatingFunc_;
}

- (id)initWithFrame:(NSRect)frame
        pixelFormat:(NSOpenGLPixelFormat*)format
       updatingFunc:(std::function<void()> const&)updatingFunc;
- (BOOL)acceptsFirstResponder;
- (BOOL)becomeFirstResponder;
- (void)animationTimer:(NSTimer*)timer;
- (void)drawRect:(NSRect)rect;
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

- (void)prepareOpenGL {
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  NSOpenGLContext* context = [self openGLContext];
  assert(context != nil);
  int const swapInterval = 1;
  [context setValues:&swapInterval forParameter:NSOpenGLCPSwapInterval];
  NSTimer* timer = [NSTimer timerWithTimeInterval:0
                                           target:self
                                         selector:@selector(animationTimer:)
                                         userInfo:nil
                                          repeats:YES];
  // NSEventTrackingRunLoopMode?
  [[NSRunLoop currentRunLoop] addTimer:timer forMode:NSDefaultRunLoopMode];
  [pool release];
}

- (BOOL)acceptsFirstResponder {
  return YES;
}

- (BOOL)becomeFirstResponder {
  return YES;
}

- (void)animationTimer:(NSTimer*)timer {
  (void)timer;
  [self setNeedsDisplay:YES];
}

- (void)drawRect:(NSRect)rect {
  (void)rect;
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  NSOpenGLContext* context = [self openGLContext];
  assert(context != nil);
  //[context makeCurrentContext];
  //[context update]; ?
  self->updatingFunc_();
  //[context clearDrawable];
  // drawing
  [context flushBuffer];
  [pool release];
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

#endif

