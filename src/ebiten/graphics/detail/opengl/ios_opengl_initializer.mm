#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_IOS_OPENGL_INITIALIZER_MM
#define EBITEN_GRAPHICS_DETAIL_OPENGL_IOS_OPENGL_INITIALIZER_MM

#include "ebiten/graphics/view.hpp"
#include <functional>

static void
ebiten_graphics_detail_initialize_opengl(ebiten::graphics::view& view,
                                         std::function<void()> const updating_func) {
  [view.native_view() setUpdatingFunc:updating_func];
}

#endif
