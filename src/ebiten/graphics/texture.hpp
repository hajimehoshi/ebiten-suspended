#ifndef EBITEN_GRAPHICS_TEXTURE_HPP
#define EBITEN_GRAPHICS_TEXTURE_HPP

#include "ebiten/platform.hpp"

#ifdef EBITEN_MACOSX
#include "ebiten/graphics/detail/opengl/texture_id.hpp"
#endif

namespace ebiten {
namespace graphics {

class texture {
public:
  typedef detail::texture_id id_type;
private:
  id_type const id_;
  std::size_t const width_;
  std::size_t const height_;
  std::size_t const texture_width_;
  std::size_t const texture_height_;
public:
  texture(id_type const& id_,
          std::size_t width_,
          std::size_t height_,
          std::size_t texture_width_,
          std::size_t texture_height_)
    : id_(id_),
      width_(width_),
      height_(height_),
      texture_width_(texture_width_),
      texture_height_(texture_height_) {
  }
  id_type
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
  std::size_t
  texture_width() const {
    return this->texture_width_;
  }
  std::size_t
  texture_height() const {
    return this->texture_height_;
  }
};

}
}

#endif

