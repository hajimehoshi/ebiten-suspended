#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_TEXTURE_FACTORY_HPP
#define EBITEN_GRAPHICS_DETAIL_OPENGL_TEXTURE_FACTORY_HPP

#include "ebiten/graphics/texture.hpp"
#include "ebiten/graphics/detail/opengl/device.hpp"
#include "ebiten/image.hpp"
#include "ebiten/noncopyable.hpp"
#include "ebiten/platform.hpp"

#ifdef EBITEN_MACOSX
# include <OpenGL/gl.h>
#endif
#ifdef EBITEN_IOS
# import <GLKit/GLKit.h>
#endif

#include <cassert>

namespace ebiten {
namespace graphics {
namespace detail {

static uint64_t
clp2(uint64_t x) {
  x -= 1;
  x |= (x >> 1);
  x |= (x >> 2);
  x |= (x >> 4);
  x |= (x >> 8);
  x |= (x >> 16);
  x |= (x >> 32);
  return x + 1;
}

class texture_factory : private noncopyable {
  friend class device;
private:
  texture_factory() {
  }
public:
  graphics::texture
  from_image(image const& image) {
    std::size_t const width  = image.width();
    std::size_t const height = image.height();
    assert(width  == clp2(width));
    assert(height == clp2(height));
    GLuint texture_id = 0;
    ::glGenTextures(1, &texture_id);
    assert(texture_id);
    ::glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    ::glBindTexture(GL_TEXTURE_2D, texture_id);
    ::glTexImage2D(GL_TEXTURE_2D,
                   0,
                   GL_RGBA,
                   static_cast<GLsizei>(width),
                   static_cast<GLsizei>(height),
                   0,
                   GL_RGBA,
                   GL_UNSIGNED_BYTE,
                   image.pixels().data());
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    ::glBindTexture(GL_TEXTURE_2D, 0);
    return graphics::texture(texture_id, width, height);
  }
  graphics::texture
  create(std::size_t width, std::size_t height) {
    std::size_t const texture_width  = clp2(width);
    std::size_t const texture_height = clp2(height);
    GLuint texture_id = 0;
    ::glGenTextures(1, &texture_id);
    ::glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    ::glBindTexture(GL_TEXTURE_2D, texture_id);
    ::glTexImage2D(GL_TEXTURE_2D,
                   0,
                   GL_RGBA,
                   static_cast<GLsizei>(texture_width),
                   static_cast<GLsizei>(texture_height),
                   0,
                   GL_RGBA,
                   GL_UNSIGNED_BYTE,
                   0);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    ::glBindTexture(GL_TEXTURE_2D, 0);
    typedef std::unique_ptr<graphics::texture const> p;
    return graphics::texture(texture_id, texture_width, texture_height);
  }
};

}
}
}

#ifdef EBITEN_TEST

namespace ebiten {
namespace graphics {
namespace detail {

BOOST_AUTO_TEST_CASE(clp2_calling) {
  BOOST_CHECK_EQUAL(256u, clp2(255));
  BOOST_CHECK_EQUAL(256u, clp2(256));
  BOOST_CHECK_EQUAL(512u, clp2(257));
}

}
}
}

#endif

#endif
