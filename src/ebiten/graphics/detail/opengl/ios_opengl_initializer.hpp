#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_IOS_OPENGL_INITIALIZER_HPP
#define EBITEN_GRAPHICS_DETAIL_OPENGL_IOS_OPENGL_INITIALIZER_HPP

#include "ebiten/graphics/detail/opengl/ios_opengl_initializer.mm"
#include "ebiten/noncopyable.hpp"

namespace ebiten {
namespace graphics {
namespace detail {

class opengl_initializer : private noncopyable {
private:
  EbitenGLKViewDelegate* delegate_;
public:
  void
  initialize(view& view,
             std::function<void()> const& updating_func) {
    GLKView* glkView = view.native_view();
    this->delegate_ = [[EbitenGLKViewDelegate alloc] init];
    [this->delegate_ setUpdatingFunc: updating_func];
    glkView.delegate = this->delegate_;
    glkView.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
  }
};

}
}
}

#endif
