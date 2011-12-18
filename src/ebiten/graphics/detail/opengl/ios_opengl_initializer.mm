#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_IOS_OPENGL_INITIALIZER_MM
#define EBITEN_GRAPHICS_DETAIL_OPENGL_IOS_OPENGL_INITIALIZER_MM

#include "ebiten/graphics/view.hpp"
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

// TODO: Fix it
static EbitenGLKViewDelegate* ebitenGlkViewDelegate;

#endif

static void
ebiten_graphics_detail_initialize_opengl(ebiten::graphics::view& view,
                                         std::function<void()> const updating_func) {
  GLKView* glkView = view.native_view();
  ebitenGlkViewDelegate = [[EbitenGLKViewDelegate alloc] init];
  [ebitenGlkViewDelegate setUpdatingFunc: updating_func];
  glkView.delegate = ebitenGlkViewDelegate;

  EAGLContext* eaglContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
  glkView.context = eaglContext;

  //[glkView setUpdatingFunc:updating_func];
}

#endif
