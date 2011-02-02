#import <Cocoa/Cocoa.h>
#include <OpenGL/gl.h>

@interface EbitenOpenGLView : NSOpenGLView {
  int i;
}

- (id)initWithFrame:(NSRect)frame pixelFormat:(NSOpenGLPixelFormat*)format;
- (void)prepareOpenGL;
- (void)animationTimer:(NSTimer*)timer;
- (void)drawRect:(NSRect)rect;

@end

@implementation EbitenOpenGLView

- (id)initWithFrame:(NSRect)frame pixelFormat:(NSOpenGLPixelFormat*)format {
  self = [super initWithFrame:frame pixelFormat:format];
  if (self != nil) {
    [self prepareOpenGL];
  }
  return self;
}

- (void)prepareOpenGL {
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  self->i = 0;
  NSOpenGLContext* context = [self openGLContext];
  assert(context != nil);
  const int swapInterval = 1;
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
  self->i++;
  if (self->i % 60 == 0) {
    NSLog(@"hoge");
  }
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  NSOpenGLContext* context = [self openGLContext];
  assert(context != nil);
  //[context makeCurrentContext];
  //[context update]; ?
  {
    ::glClearColor(0, 0, 0, 1);
    ::glClear(GL_COLOR_BUFFER_BIT);        
    ::glEnable(GL_TEXTURE_2D);
    ::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    ::glEnable(GL_BLEND);
    //::glViewport(0, 0, screen_width, screen_height);
    ::glMatrixMode(GL_PROJECTION);
    ::glLoadIdentity();
    //::glOrtho(0, screen_width, 0, screen_height, 0, 1);
    ::glFlush();
  }
  //[context clearDrawable];
  // drawing
  [context flushBuffer];
  [pool release];
}

@end
