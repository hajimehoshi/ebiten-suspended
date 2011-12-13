#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_IOS_OPENGL_INITIALIZER_MM
#define EBITEN_GRAPHICS_DETAIL_OPENGL_IOS_OPENGL_INITIALIZER_MM

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

@interface EbitenOpenGLViewController : GLKViewController {
@private
  EAGLContext* eaglContext_;
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
  self->eaglContext_ = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
  assert(self->eaglContext_);
  GLKView* view = (GLKView*)self.view;
  view.context = self->eaglContext_;
  // view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
  /*GLuint viewFramebuffer, viewRenderbuffer;
  ::glGenFramebuffers(1, &viewFramebuffer);
  ::glGenRenderbuffers(1, &viewRenderbuffer);
  ::glBindFramebuffer(GL_FRAMEBUFFER_OES, viewFramebuffer);
  ::glBindRenderbuffer(GL_RENDERBUFFER_OES, viewRenderbuffer);
  [self->eaglContext_ renderbufferStorage:GL_RENDERBUFFER
                             fromDrawable:(CAEAGLLayer*)self.layer];
  ::glFramebufferRenderbuffer(GL_FRAMEBUFFER_OES,
                              GL_COLOR_ATTACHMENT0_OES,
                              GL_RENDERBUFFER_OES,
                              viewRenderbuffer);*/
}

- (void)glkView:(GLKView*)view
     drawInRect:(CGRect)rect {
  if (!self->updatingFunc_) {
    return;
  }
  self->updatingFunc_();
}

@end
#endif

static void
ebiten_graphics_detail_initialize_opengl_with_view(EbitenOpenGLViewController* view,
                                                   std::function<void()> const updating_func) {
  [view setUpdatingFunc:updating_func];
}

#endif
