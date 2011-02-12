#ifndef EBITEN_GAME_GRAPHICS_OPENGL_TEXTURE_FACTORY_HPP
#define EBITEN_GAME_GRAPHICS_OPENGL_TEXTURE_FACTORY_HPP

#include "ebiten/game/graphics/texture.hpp"
#include "ebiten/util/id.hpp"
#include "ebiten/util/image_loader.hpp"
#include "ebiten/util/singleton.hpp"
#include <memory>
#include <OpenGL/gl.h>

namespace ebiten {
namespace game {
namespace graphics {
namespace opengl {

// TODO: implements for 32bit machine
BOOST_STATIC_ASSERT(sizeof(uint64_t) == sizeof(std::size_t));

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

class texture_factory : public util::singleton<texture_factory> {
  friend class util::singleton<texture_factory>;
public:
  std::unique_ptr<graphics::texture>
  from_file(const std::string& filename) {
    const auto image = util::image_loader::instance().load_file(filename);
    const auto width  = image->width();
    const auto height = image->height();
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
                   image->pixels().data());
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    ::glBindTexture(GL_TEXTURE_2D, 0);
    typedef graphics::texture texture;
    return std::unique_ptr<texture>(new texture(util::id_(texture_id),
                                                width,
                                                height,
                                                width,
                                                height));
  }
  std::unique_ptr<graphics::texture>
  create(std::size_t width, std::size_t height) {
    const auto texture_width  = clp2(width);
    const auto texture_height = clp2(height);
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
    typedef graphics::texture texture;
    return std::unique_ptr<texture>(new texture(util::id_(texture_id),
                                                width,
                                                height,
                                                texture_width,
                                                texture_height));
  }
private:
  texture_factory() {
  }
  ~texture_factory() {
  }
};

}
}
}
}

#ifdef EBITEN_TEST

namespace ebiten {
namespace game {
namespace graphics {
namespace opengl {

TEST(clp2, calling) {
  EXPECT_EQ(256, clp2(255));
  EXPECT_EQ(256, clp2(256));
  EXPECT_EQ(512, clp2(257));
}

}
}
}
}

#endif

#endif

