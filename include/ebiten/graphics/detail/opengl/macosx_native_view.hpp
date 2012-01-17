#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_MACOSX_NATIVE_VIEW_HPP
#define EBITEN_GRAPHICS_DETAIL_OPENGL_MACOSX_NATIVE_VIEW_HPP

#include <functional>

namespace ebiten {
namespace graphics {
namespace detail {
namespace {

void
set_input_funcs(EbitenOpenGLView* v,
                std::function<void(int, int, int)> const& setting_touches_location_func,
                std::function<void(int, bool)> const& setting_touched_func) {
  [v setSettingTouchesLocationFunc:setting_touches_location_func];
  [v setSettingTouchedFunc:setting_touched_func];
}

}
}
}
}

#endif
