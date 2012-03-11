#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_IOS_OPENGL_INITIALIZER_HPP
#define EBITEN_GRAPHICS_DETAIL_OPENGL_IOS_OPENGL_INITIALIZER_HPP

#include "ebiten/graphics/detail/opengl/ios_native_view.mm"
#include "ebiten/graphics/native_view.hpp"
#include "ebiten/noncopyable.hpp"

namespace ebiten {
namespace graphics {
namespace detail {

class opengl_initializer : private noncopyable {
private:
  native_view native_view_;
  EbitenGLKViewDelegate* delegate_;
public:
  opengl_initializer(native_view native_view)
    : native_view_(native_view) {
  }
  void
  initialize(std::function<bool()> const& updating_func) {
    GLKView* glkView = this->native_view_;
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
