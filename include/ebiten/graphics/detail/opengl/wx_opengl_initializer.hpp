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
public:
  void
  initialize(native_view native_view,
             std::function<bool()> const& updating_func) {
    // TODO: Use the return value of updating_func
    native_view->Bind(wxEVT_PAINT, std::bind(updating_func));
  }
};

}
}
}

#endif
