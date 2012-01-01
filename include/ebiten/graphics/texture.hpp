#ifndef EBITEN_GRAPHICS_TEXTURE_HPP
#define EBITEN_GRAPHICS_TEXTURE_HPP

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

class texture {
private:
  bool is_null_;
  detail::texture_id id_;
  std::size_t width_;
  std::size_t height_;
  std::size_t texture_width_;
  std::size_t texture_height_;
  friend class detail::device;
  friend class detail::graphics_context;
  friend class detail::texture_factory;
public:
  // TODO: Is 0 a magic number?
  texture()
    : is_null_(true),
      id_(0),
      width_(0),
      height_(0),
      texture_width_(0),
      texture_height_(0) {
  }
  texture(texture const&) = default;
  texture& operator=(texture const&) = default;
  texture(texture&& rhs)
    : is_null_(std::move(rhs.is_null_)),
      id_(std::move(rhs.id_)),
      width_(std::move(rhs.width_)),
      height_(std::move(rhs.height_)),
      texture_width_(std::move(rhs.texture_width_)),
      texture_height_(std::move(rhs.texture_height_)) {
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
  operator bool() const {
    return !this->is_null_;
  }
private:
  texture(detail::texture_id const& id_,
          std::size_t width_,
          std::size_t height_,
          std::size_t texture_width_,
          std::size_t texture_height_)
    : is_null_(false),
      id_(id_),
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
