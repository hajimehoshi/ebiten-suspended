#ifndef EBITEN_GRAPHICS_GRAPHICS_VIEW_HPP
#define EBITEN_GRAPHICS_GRAPHICS_VIEW_HPP

#if defined(EBITEN_MACOSX) || defined(EBITEN_IOS)
# include "ebiten/graphics/detail/opengl/view.hpp"
#endif

#include "ebiten/noncopyable.hpp"

namespace ebiten {
namespace graphics {

typedef detail::native_view native_view;

class view : private noncopyable {
private:
  native_view const native_view_;
public:
  view(native_view n)
    : native_view_(n) {
  }
  native_view
  native_view() {
    return this->native_view_;
  }
};

}
}

#endif
