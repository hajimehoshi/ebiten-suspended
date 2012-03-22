#ifndef EBITEN_GRAPHICS_DETAIL_OPENGL_TEXTURE_HPP
#define EBITEN_GRAPHICS_DETAIL_OPENGL_TEXTURE_HPP

#include "ebiten/noncopyable.hpp"
#include "ebiten/platform.hpp"

#ifdef EBITEN_MACOSX
# include <OpenGL/gl.h>
#endif
#ifdef EBITEN_IOS
# import <GLKit/GLKit.h>
#endif

namespace ebiten {
namespace graphics {
namespace detail {

class device;
class graphics_context;
class texture_factory;

typedef GLuint texture_id;

class texture : private noncopyable {
private:
  texture_id const id_;
  std::size_t const width_;
  std::size_t const height_;
  std::size_t const texture_width_;
  std::size_t const texture_height_;
private:
  friend class device;
  friend class graphics_context;
  friend class texture_factory;
public:
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
  texture(texture_id const& id_,
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
  texture_id const&
  id() const {
    return this->id_;
  }
};

}
}
}

// TODO: implement operator= (with nullptr), swap, and so on?

#endif
