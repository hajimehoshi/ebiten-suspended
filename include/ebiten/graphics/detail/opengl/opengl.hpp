#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_OPENGL_HPP
#define EBITEN_GRAPHICS_DETAIL_OPENGL_OPENGL_HPP

#include "ebiten/platform.hpp"

#ifdef _WIN32
# include <GL/gl.h>
# include <GL/glu.h>
#endif
#ifdef EBITEN_MACOSX
# include <OpenGL/gl.h>
#endif
#ifdef EBITEN_IOS
# import <GLKit/GLKit.h>
#endif

#endif
