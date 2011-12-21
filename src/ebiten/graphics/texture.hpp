#ifndef EBITEN_GRAPHICS_TEXTURE_HPP
#define EBITEN_GRAPHICS_TEXTURE_HPP

#include "ebiten/platform.hpp"

#if defined(EBITEN_MACOSX) || defined(EBITEN_IOS)
#include "ebiten/graphics/detail/opengl/texture_id.hpp"
#endif

#include <cstddef>

namespace ebiten {
namespace graphics {

namespace detail {

class device;
class graphics_context;
class texture_factory;

}

class texture {
private:
  detail::texture_id id_;
  std::size_t width_;
  std::size_t height_;
  friend class detail::device;
  friend class detail::graphics_context;
  friend class detail::texture_factory;
public:
  // TODO: Is 0 a magic number?
  texture()
    : id_(0),
      width_(0),
      height_(0) {
  }
  texture(texture const&) = default;
  texture& operator=(texture const&) = default;
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
private:
  texture(detail::texture_id const& id_,
          std::size_t width_,
          std::size_t height_)
    : id_(id_),
      width_(width_),
      height_(height_) {
  }
  detail::texture_id const&
  id() const {
    return this->id_;
  }
};

}
}

// TODO: implement operator= (with nullptr), swap, and so on?

#endif
