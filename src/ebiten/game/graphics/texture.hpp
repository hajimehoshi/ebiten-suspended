#ifndef EBITEN_GAME_GRAPHICS_TEXTURE_HPP
#define EBITEN_GAME_GRAPHICS_TEXTURE_HPP

#include "ebiten/util/id.hpp"
#include <boost/any.hpp>
#include <boost/noncopyable.hpp>

namespace ebiten {
namespace game {
namespace graphics {

// texture というよりは texture_id?
class texture : private boost::noncopyable {
private:
  const util::id_ id_;
  const std::size_t width_;
  const std::size_t height_;
  const std::size_t texture_width_;
  const std::size_t texture_height_;
public:
  texture(util::id_ id_,
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
  util::id_
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
}

#endif

