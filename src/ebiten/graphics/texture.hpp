#ifndef EBITEN_GRAPHICS_TEXTURE_HPP
#define EBITEN_GRAPHICS_TEXTURE_HPP

#include "ebiten/platform.hpp"

// TODO: define EBITEN_OPENGL / EBITEN_OPENGLES?
#if defined(EBITEN_MACOSX) || defined(EBITEN_IOS)
#include "ebiten/graphics/detail/opengl/texture_id.hpp"
#endif

#include <cstddef>

namespace ebiten {
namespace graphics {

typedef detail::texture_id texture_id;

class texture {
private:
  texture_id id_;
  std::size_t width_;
  std::size_t height_;
public:
  // TODO: Is 0 a magic number?
  texture()
    : id_(0),
      width_(0),
      height_(0) {
  }
  texture(texture const&) = default;
  texture& operator=(texture const&) = default;
  texture(texture_id const& id_,
          std::size_t width_,
          std::size_t height_)
    : id_(id_),
      width_(width_),
      height_(height_) {
  }
  texture_id
  id() const {
    return this->id_;
  }
  std::size_t
  width() const {
    return this->width_;
  }
  std::size_t
  height() const {
    return this->height_;
  }
  operator bool() const {
    return this->id_ != 0;
  }
};

}
}

// TODO: implement operator= (with nullptr), swap, and so on?

#endif
