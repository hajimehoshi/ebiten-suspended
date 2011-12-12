#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_TEXTURE_ID_HPP
#define EBITEN_GRAPHICS_DETAIL_OPENGL_TEXTURE_ID_HPP

#ifdef EBITEN_MACOSX
# include <OpenGL/gl.h>
#endif
#ifdef EBITEN_IOS
# import <GLKit/GLKit.h>
#endif

namespace ebiten {
namespace graphics {
namespace detail {

typedef GLuint texture_id;

}
}
}

#endif
