#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_IOS_OPENGL_INITIALIZER_MM
#define EBITEN_GRAPHICS_DETAIL_OPENGL_IOS_OPENGL_INITIALIZER_MM

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#include <functional>

@interface EbitenGLKViewDelegate : NSObject<GLKViewDelegate> {
@private
  std::function<void()> updatingFunc_;
}

- (void)setUpdatingFunc:(std::function<void()> const&)updatingFunc;
- (void)glkView:(GLKView*)view
     drawInRect:(CGRect)rect;
@end

#ifndef EBITEN_WITHOUT_OBJC_IMPL
@implementation EbitenGLKViewDelegate

- (void)setUpdatingFunc:(std::function<void()> const&)updatingFunc {
  self->updatingFunc_ = updatingFunc;
}

- (void)glkView:(GLKView*)view
     drawInRect:(CGRect)rect {
  if (!self->updatingFunc_) {
    return;
  }
  [EAGLContext setCurrentContext:[view context]];
  self->updatingFunc_();
}

@end
#endif

#endif
