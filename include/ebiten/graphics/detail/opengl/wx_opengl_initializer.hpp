#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_WX_OPENGL_INITIALIZER_HPP
#define EBITEN_GRAPHICS_DETAIL_OPENGL_WX_OPENGL_INITIALIZER_HPP

#include "ebiten/graphics/native_view.hpp"
#include "ebiten/noncopyable.hpp"
#include <functional>
#include <memory>

namespace ebiten {
namespace graphics {
namespace detail {

class opengl_initializer : private noncopyable {
private:
  native_view native_view_;
  wxGLContext context_;
public:
  opengl_initializer(native_view native_view)
    : native_view_(native_view),
      context_(native_view) {
  }
  void
  initialize(std::function<bool()> const& updating_func) {
    struct updating_func_wrapper {
      wxGLCanvas* native_view_;
      wxGLContext* context_;
      std::function<bool()> updating_func_;
      updating_func_wrapper(wxGLCanvas* native_view,
                            wxGLContext* context,
                            std::function<bool()> const& updating_func)
        : native_view_(native_view),
          context_(context),
          updating_func_(updating_func) {
      }
      void
      operator() (wxEvent&) {
        if (!this->native_view_->IsShownOnScreen()) {
          return;
        }
        wxPaintDC dc(this->native_view_);
        this->native_view_->SetCurrent(*this->context_);
        // TODO: Use the return value of updating_func
        this->updating_func_();
        this->native_view_->SwapBuffers();
        return;
      }
    } func(this->native_view_, &this->context_, updating_func);
    this->native_view_->Bind(wxEVT_PAINT, func);
  }
};

}
}
}

#endif
