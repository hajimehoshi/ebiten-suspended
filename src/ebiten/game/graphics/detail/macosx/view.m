#import <Cocoa/Cocoa.h>
#undef check # conflicts with boost
#include <boost/function.hpp>
#include <boost/signals2/signal.hpp>

@interface EbitenOpenGLView : NSOpenGLView {
  boost::signals2::signal<void()> updatingDevice;
}

- (id)initWithFrame:(NSRect)frame
        pixelFormat:(NSOpenGLPixelFormat*)format;
- (void)prepareOpenGL;
- (void)animationTimer:(NSTimer*)timer;
- (void)drawRect:(NSRect)rect;
- (void)connectUpdatingDevice:(boost::function<void()> const &)func;

@end

@implementation EbitenOpenGLView

- (id)initWithFrame:(NSRect)frame
        pixelFormat:(NSOpenGLPixelFormat*)format {
  self = [super initWithFrame:frame pixelFormat:format];
  if (self != nil) {
    [self prepareOpenGL];
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
  self->updatingDevice();
  //[context clearDrawable];
  // drawing
  [context flushBuffer];
  [pool release];
}

- (void)connectUpdatingDevice:(boost::function<void()> const &)func {
  self->updatingDevice.connect(func);
}

@end
