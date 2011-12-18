#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_IOS_VIEW_MM
#define EBITEN_GRAPHICS_DETAIL_OPENGL_IOS_VIEW_MM

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#include <functional>

@interface EbitenOpenGLViewController : GLKViewController {
@private
  EAGLContext* context_;
  std::function<void()> updatingFunc_;
}
- (void)setUpdatingFunc:(std::function<void()> const&)updatingFunc;
@end

#ifndef EBITEN_WITHOUT_OBJC_IMPL
@implementation EbitenOpenGLViewController

- (void)setUpdatingFunc:(std::function<void()> const&)updatingFunc {
  self->updatingFunc_ = updatingFunc;
}

- (void)viewDidLoad {
  [super viewDidLoad];
  self->context_ = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
  assert(self->context_);
  [EAGLContext setCurrentContext:self->context_];
  GLKView* view = (GLKView*)self.view;
  view.context = self->context_;
  view.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
  view.drawableDepthFormat = GLKViewDrawableDepthFormat16;
  // self.preferredFramesPerSecond = 30;
  NSLog(@"Syokika");
}

- (void)glkView:(GLKView*)view
     drawInRect:(CGRect)rect {
  if (!self->updatingFunc_) {
    return;
  }
  if (!self->context_) {
    return;
  }
  [EAGLContext setCurrentContext:self->context_];
  self->updatingFunc_();
}

@end
#endif

#endif

