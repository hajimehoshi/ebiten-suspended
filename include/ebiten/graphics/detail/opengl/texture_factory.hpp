#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_TEXTURE_FACTORY_HPP
#define EBITEN_GRAPHICS_DETAIL_OPENGL_TEXTURE_FACTORY_HPP

#include "ebiten/graphics/texture.hpp"
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
#include <memory>

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

class device;

class texture_factory : private noncopyable {
  friend class texture;
  friend class device;
private:
  struct texture_deleter {
    void
    operator()(class texture* texture) const {
      ::glDeleteTextures(1, &texture->id());
      delete texture;
    }
  };
public:
  typedef std::unique_ptr<class texture, texture_deleter> texture_pointer;
private:
  texture_factory() {
  }
public:
  texture_pointer
  from_image(image const& image) {
    std::size_t const width  = image.width();
    std::size_t const height = image.height();
    std::size_t const texture_width  = clp2(width);
    std::size_t const texture_height = clp2(height);
    GLuint texture_id = 0;
    ::glGenTextures(1, &texture_id);
    assert(texture_id);
    ::glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    ::glBindTexture(GL_TEXTURE_2D, texture_id);
    {
      std::vector<uint8_t> pixels(texture_width * texture_height * 4, 0);
      image.pixels_with_padding(pixels, texture_width, texture_height);
      ::glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGBA,
                     static_cast<GLsizei>(texture_width),
                     static_cast<GLsizei>(texture_height),
                     0,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     pixels.data());
    }
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    ::glBindTexture(GL_TEXTURE_2D, 0);
    typedef graphics::texture t;
    texture_pointer p(new t(texture_id, width, height, texture_width, texture_height));
    return p;
  }
  std::unique_ptr<graphics::texture, texture_deleter>
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
    typedef graphics::texture t;
    texture_pointer p(new t(texture_id, width, height, texture_width, texture_height));
    return p;
  }
};

}
}
}

#ifdef EBITEN_TEST

namespace ebiten {
namespace graphics {
namespace detail {

TEST(clp2, calling) {
  EXPECT_EQ(256u, clp2(255));
  EXPECT_EQ(256u, clp2(256));
  EXPECT_EQ(512u, clp2(257));
}

}
}
}

#endif

#endif
