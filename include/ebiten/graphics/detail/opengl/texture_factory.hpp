#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_TEXTURE_FACTORY_HPP
#define EBITEN_GRAPHICS_DETAIL_OPENGL_TEXTURE_FACTORY_HPP

#include "ebiten/graphics/detail/opengl/opengl.hpp"
#include "ebiten/graphics/texture.hpp"
#include "ebiten/image.hpp"
#include "ebiten/noncopyable.hpp"
#include <cassert>
#include <functional>
#include <memory>

namespace ebiten {
namespace graphics {
namespace detail {

namespace {

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

}

class device;

class texture_factory : private noncopyable {
  friend class texture;
  friend class device;
private:
  texture_factory() {
  }
public:
  texture
  from_image(image const& img) {
    std::size_t const width  = img.width();
    std::size_t const height = img.height();
    std::size_t const texture_width  = clp2(width);
    std::size_t const texture_height = clp2(height);
    GLuint texture_id = 0;
    ::glGenTextures(1, &texture_id);
    assert(texture_id);
    ::glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    ::glBindTexture(GL_TEXTURE_2D, texture_id);
    if (width == texture_width && height == texture_height) {
      this->gl_tex_image2d(img);
    } else {
      image const img_with_padding(img, texture_width, texture_height);
      this->gl_tex_image2d(img_with_padding);
    }
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    //::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    ::glBindTexture(GL_TEXTURE_2D, 0);
    return std::move(texture(texture_id, width, height, texture_width, texture_height));
  }
private:
  void
  gl_tex_image2d(image const& img_with_padding) {
    ::glTexImage2D(GL_TEXTURE_2D,
                   0,
                   GL_RGBA,
                   static_cast<GLsizei>(img_with_padding.width()),
                   static_cast<GLsizei>(img_with_padding.height()),
                   0,
                   GL_RGBA,
                   GL_UNSIGNED_BYTE,
                   img_with_padding.pixels().data());
  }
public:
  texture
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
                   nullptr);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    //::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    ::glBindTexture(GL_TEXTURE_2D, 0);
    // これが滅びると同時にテクスチャも滅びる必要がある?
    return std::move(texture(texture_id, width, height, texture_width, texture_height));
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
