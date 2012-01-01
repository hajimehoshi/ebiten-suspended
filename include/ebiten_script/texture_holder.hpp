#ifndef EBITEN_SCRIPT_TEXTURE_HOLDER_HPP
#define EBITEN_SCRIPT_TEXTURE_HOLDER_HPP

#include "ebiten/graphics/texture.hpp"
#include "ebiten/graphics/texture_factory.hpp"
#include "ebiten/image.hpp"

#include "ebiten/noncopyable.hpp"
#include <string>

namespace ebiten_script {

class texture_holder : private ebiten::noncopyable {
private:
  ebiten::graphics::texture ebiten_texture_;
  std::string const path_;
  std::size_t const width_;
  std::size_t const height_;
public:
  texture_holder(std::string const& path)
    : path_(path),
      width_(0),
      height_(0) {
  }
  texture_holder(std::size_t width, std::size_t height)
    : path_(),
      width_(width),
      height_(height) {
  }
  texture_holder(texture_holder&& rhs)
    : ebiten_texture_(std::move(rhs.ebiten_texture_)),
      path_(std::move(rhs.path_)),
      width_(std::move(rhs.width_)),
      height_(std::move(rhs.height_)) {
  }
  void
  instantiate(ebiten::graphics::texture_factory& tf) {
    if (this->ebiten_texture_) {
      return;
    }
    if (!this->path_.empty()) {
      std::unique_ptr<ebiten::image> image =
        ebiten::image_loader::load_png(this->path_);
      this->ebiten_texture_ = tf.from_image(*image);
    } else if (0 < this->width_ && 0 < this->height_) {
      this->ebiten_texture_ = tf.create(this->width_, this->height_);
    }
  }
  ebiten::graphics::texture&
  ebiten_texture() {
    return this->ebiten_texture_;
  }
  ebiten::graphics::texture const&
  ebiten_texture() const {
    return this->ebiten_texture_;
  }
  void
  set_ebiten_texture(ebiten::graphics::texture const& ebiten_texture) {
    this->ebiten_texture_ = ebiten_texture;
  }
};

}

#endif
