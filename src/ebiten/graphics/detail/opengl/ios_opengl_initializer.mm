#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_IOS_OPENGL_INITIALIZER_MM
#define EBITEN_GRAPHICS_DETAIL_OPENGL_IOS_OPENGL_INITIALIZER_MM

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

@interface EbitenOpenGLViewController : GLKViewController<GLKViewControllerDelegate, GLKViewDelegate> {
@private
  EAGLContext* context_;
  GLKBaseEffect* effect_;
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
  view.delegate = self;
  view.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
  view.drawableDepthFormat = GLKViewDrawableDepthFormat16;

  self.delegate = self;
  self.preferredFramesPerSecond = 30;

  self->effect_ = [[GLKBaseEffect alloc] init];
}

- (void)glkViewControllerUpdate:(GLKViewController *)controller {
  /*static float transY = 0.0f;
  float y = sinf(transY)/2.0f;
  transY += 0.175f;

  GLKMatrix4 modelview = GLKMatrix4MakeTranslation(0, y, -5.0f);
  effect.transform.modelviewMatrix = modelview;

  GLfloat ratio = self.view.bounds.size.width/self.view.bounds.size.height;
  GLKMatrix4 projection = GLKMatrix4MakePerspective(45.0f, ratio, 0.1f, 20.0f);
  effect.transform.projectionMatrix = projection;*/
  // update?
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

static void
ebiten_graphics_detail_initialize_opengl_with_view(EbitenOpenGLViewController* view,
                                                   std::function<void()> const updating_func) {
  [view setUpdatingFunc:updating_func];
}

#endif
