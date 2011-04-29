#import <Cocoa/Cocoa.h>
#undef check # conflicts with boost
#include <boost/function.hpp>

@interface EbitenOpenGLView : NSOpenGLView {
  boost::function<void()> updateDevice;
}

- (id)initWithFrame:(NSRect)frame
        pixelFormat:(NSOpenGLPixelFormat*)format
       updateDevice:(boost::function<void()>)updateDevice_;
- (void)prepareOpenGL;
- (void)animationTimer:(NSTimer*)timer;
- (void)drawRect:(NSRect)rect;

@end

@implementation EbitenOpenGLView

- (id)initWithFrame:(NSRect)frame
        pixelFormat:(NSOpenGLPixelFormat*)format
       updateDevice:(boost::function<void()>)updateDevice_ {
  self = [super initWithFrame:frame pixelFormat:format];
  if (self != nil) {
    self->updateDevice = updateDevice_;
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
  self->updateDevice();
  //[context clearDrawable];
  // drawing
  [context flushBuffer];
  [pool release];
}

@end
