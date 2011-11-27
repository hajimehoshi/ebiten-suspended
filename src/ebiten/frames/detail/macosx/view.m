#import <Cocoa/Cocoa.h>
#include <functional>

@interface EbitenOpenGLView : NSOpenGLView {
  std::function<void()> updatingFunc_;
}

- (id)initWithFrame:(NSRect)frame
        pixelFormat:(NSOpenGLPixelFormat*)format
       updatingFunc:(std::function<void()> const&)updatingFunc;
- (void)prepareOpenGL;
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
  [[NSRunLoop currentRunLoop] addTimer:timer forMode:NSDefaultRunLoopMode];
  //[[NSRunLoop currentRunLoop] addTimer:timer forMode:NSEventTrackingRunLoopMode];
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
