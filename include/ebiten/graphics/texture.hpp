#ifndef EBITEN_GRAPHICS_TEXTURE_HPP
#define EBITEN_GRAPHICS_TEXTURE_HPP

#include "ebiten/noncopyable.hpp"
#include "ebiten/platform.hpp"

#if defined(EBITEN_MACOSX) || defined(EBITEN_IOS)
#include "ebiten/graphics/detail/opengl/texture_id.hpp"
#endif

#include <cstddef>
#include <utility>

namespace ebiten {
namespace graphics {

namespace detail {

class device;
class graphics_context;
class texture_factory;

}

class texture : private noncopyable {
private:
  detail::texture_id const id_;
  std::size_t const width_;
  std::size_t const height_;
  std::size_t const texture_width_;
  std::size_t const texture_height_;
  friend class detail::device;
  friend class detail::graphics_context;
  friend class detail::texture_factory;
public:
  ~texture() {
    // TODO: implement or set deleter!
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
private:
  texture(detail::texture_id const& id_,
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
  detail::texture_id const&
  id() const {
    return this->id_;
  }
};

}
}

// TODO: implement operator= (with nullptr), swap, and so on?

#endif
