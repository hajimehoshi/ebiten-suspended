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
- (void)setInput:(ebiten::input&)input;
- (void)touchesBegan:(NSSet*)touches
           withEvent:(UIEvent*)event
            withView:(UIView*)view;
- (void)touchesMoved:(NSSet*)touches
           withEvent:(UIEvent*)event
            withView:(UIView*)view;
- (void)touchesEnded:(NSSet*)touches
           withEvent:(UIEvent*)event
            withView:(UIView*)view;
- (void)touchesCancelled:(NSSet*)touches
               withEvent:(UIEvent*)event
                withView:(UIView*)view;

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
           withEvent:(UIEvent*)event
            withView:(UIView*)view {
  if ([touches count] == 0) {
    self->input_->set_touches_real_location(0, -1, -1);
    self->input_->set_touched(0, false);
    return;
  }
  int sumX = 0;
  int sumY = 0;
  for (UITouch* touch in touches) {
    CGPoint location = [touch locationInView:view];
    sumX += location.x;
    sumY += location.y;
  }
  int x = sumX / [touches count];
  int y = sumY / [touches count];
  self->input_->set_touches_real_location(0, x, y);
  self->input_->set_touched(0, true);
}

- (void)touchesMoved:(NSSet*)touches
           withEvent:(UIEvent*)event
            withView:(UIView*)view {
  if ([touches count] == 0) {
    self->input_->set_touches_real_location(0, -1, -1);
    self->input_->set_touched(0, false);
    return;
  }
  int sumX = 0;
  int sumY = 0;
  for (UITouch* touch in touches) {
    CGPoint location = [touch locationInView:view];
    sumX += location.x;
    sumY += location.y;
  }
  int x = sumX / [touches count];
  int y = sumY / [touches count];
  self->input_->set_touches_real_location(0, x, y);
  self->input_->set_touched(0, true);
}

- (void)touchesEnded:(NSSet*)touches
           withEvent:(UIEvent*)event
            withView:(UIView*)view {
  self->input_->set_touches_real_location(0, -1, -1);
  self->input_->set_touched(0, false);
}

- (void)touchesCancelled:(NSSet*)touches
               withEvent:(UIEvent*)event
                withView:(UIView*)view {
  self->input_->set_touches_real_location(0, -1, -1);
  self->input_->set_touched(0, false);
}

@end
#endif

@interface EbitenGLKViewController : GLKViewController
@end

#ifndef EBITEN_WITHOUT_OBJC_IMPL
@implementation EbitenGLKViewController

- (void)touchesBegan:(NSSet*)touches
           withEvent:(UIEvent*)event {
  EbitenGLKViewDelegate* delegate = ((GLKView*)self.view).delegate;
  [delegate touchesBegan:touches
               withEvent:event
                withView:self.view];
}

- (void)touchesMoved:(NSSet*)touches
           withEvent:(UIEvent*)event {
  EbitenGLKViewDelegate* delegate = ((GLKView*)self.view).delegate;
  [delegate touchesMoved:touches
               withEvent:event
                withView:self.view];
}

- (void)touchesEnded:(NSSet*)touches
           withEvent:(UIEvent*)event {
  EbitenGLKViewDelegate* delegate = ((GLKView*)self.view).delegate;
  [delegate touchesEnded:touches
               withEvent:event
                withView:self.view];
}

- (void)touchesCancelled:(NSSet*)touches
               withEvent:(UIEvent*)event {
  EbitenGLKViewDelegate* delegate = ((GLKView*)self.view).delegate;
  [delegate touchesCancelled:touches
                   withEvent:event
                    withView:self.view];
}

@end
#endif

#endif
