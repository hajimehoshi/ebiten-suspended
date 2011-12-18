#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_OPENGL_INITIALIZER_HPP
#define EBITEN_GRAPHICS_DETAIL_OPENGL_OPENGL_INITIALIZER_HPP

#include "ebiten/platform.hpp"

#ifdef EBITEN_MACOSX
# include "ebiten/graphics/detail/opengl/macosx_opengl_initializer.hpp"
#endif

#ifdef EBITEN_IOS
# include "ebiten/graphics/detail/opengl/ios_opengl_initializer.hpp"
#endif

#endif
