#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_TEXTURE_FACTORY_HPP
#define EBITEN_GRAPHICS_DETAIL_OPENGL_TEXTURE_FACTORY_HPP

#include "ebiten/graphics/texture.hpp"
#include "ebiten/graphics/detail/opengl/device.hpp"
#include "ebiten/util/image.hpp"
#include "ebiten/util/image_loader.hpp"
#include <OpenGL/gl.h>
#include <cassert>

namespace ebiten {
namespace graphics {
namespace detail {

// TODO: implements for 32bit machine
static_assert(sizeof(uint64_t) == sizeof(std::size_t),
              "sizeof(uint64_t) must equal to sizeof(std::size_t)");

// TODO: implements for 32bit machine
uint64_t
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

class texture_factory : public boost::noncopyable {
  friend class device;
private:
  texture_factory() {
  }
public:
  graphics::texture
  from_file(std::string const& filename) {
    util::image image(util::png_image_loader, filename);
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
                   width,
                   height,
                   0,
                   GL_RGBA,
                   GL_UNSIGNED_BYTE,
                   image.pixels().data());
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    ::glBindTexture(GL_TEXTURE_2D, 0);
    return graphics::texture(texture_id,
                             width,
                             height,
                             width,
                             height);
  }
  graphics::texture
  create(std::size_t width, std::size_t height) {
    std::size_t const texture_width  = clp2(width);
    std::size_t const texture_height = clp2(height);
    GLuint texture_id = 0;
    ::glGenTextures(1, &texture_id);
    assert(texture_id);
    ::glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    ::glBindTexture(GL_TEXTURE_2D, texture_id);
    ::glTexImage2D(GL_TEXTURE_2D,
                   0,
                   GL_RGBA,
                   texture_width,
                   texture_height,
                   0,
                   GL_RGBA,
                   GL_UNSIGNED_BYTE,
                   0);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    ::glBindTexture(GL_TEXTURE_2D, 0);
    return graphics::texture(texture_id,
                             width,
                             height,
                             texture_width,
                             texture_height);
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
