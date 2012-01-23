#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_IOS_NATIVE_VIEW_MM
#define EBITEN_GRAPHICS_DETAIL_OPENGL_IOS_NATIVE_VIEW_MM

#include "ebiten/input.hpp"
#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#include <functional>

@interface EbitenGLKViewDelegate : NSObject<GLKViewDelegate> {
@private
  std::function<bool()> updatingFunc_;
  ebiten::input* input_;
}

- (void)setUpdatingFunc:(std::function<bool()> const&)updatingFunc;
- (void)glkView:(GLKView*)view
     drawInRect:(CGRect)rect;
@end

#ifndef EBITEN_WITHOUT_OBJC_IMPL
@implementation EbitenGLKViewDelegate

- (void)setUpdatingFunc:(std::function<bool()> const&)updatingFunc {
  self->updatingFunc_ = updatingFunc;
}

- (void)setInput:(ebiten::input&)input {
  self->input_ = &input;
}

- (void)glkView:(GLKView*)view
     drawInRect:(CGRect)rect {
  if (!self->updatingFunc_) {
    return;
  }
  [EAGLContext setCurrentContext:[view context]];
  self->updatingFunc_();
  // TODO: Terminating
}

- (void)touchesBegan:(NSSet*)touches
           withEvent:(UIEvent*)event {
  NSLog(@"delegate touchesBegan");
}

- (void)touchesMoved:(NSSet*)touches
           withEvent:(UIEvent*)event {
  NSLog(@"delegate touchesMoved");
}

- (void)touchesEnded:(NSSet*)touches
           withEvent:(UIEvent*)event {
  NSLog(@"delegate touchesEnded");
}

- (void)touchesCancelled:(NSSet*)touches
               withEvent:(UIEvent*)event {
  NSLog(@"delegate touchesCancelled");
}

@end
#endif

#endif
