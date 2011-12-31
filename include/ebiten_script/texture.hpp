#ifndef EBITEN_SCRIPT_TEXTURE_HPP
#define EBITEN_SCRIPT_TEXTURE_HPP

#include "ebiten/graphics/texture.hpp"
#include "ebiten/graphics/texture_factory.hpp"
#include "ebiten/image.hpp"
#include "ebiten/image_loader.hpp"
#include "ebiten/noncopyable.hpp"
#include <string>

namespace ebiten_script {

class texture : private ebiten::noncopyable {
private:
  ebiten::graphics::texture ebiten_texture_;
  std::string path_;
  std::size_t width_;
  std::size_t height_;
public:
  texture()
    : width_(0),
      height_(0) {
  }
  void
  set_path(std::string const& path) {
    this->path_ = path;
  }
  void
  set_size(std::size_t width, std::size_t height) {
    this->width_  = width;
    this->height_ = height;
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
    } else {
      this->ebiten_texture_ = tf.create(this->width_, this->height_);
    }
  }
  void
  draw(ebiten::graphics::graphics_context&) {
    // TODO: implement
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
  bool
  is_created() const {
    return this->ebiten_texture_;
  }
  std::size_t
  width() const {
    return this->ebiten_texture_.width();
  }
  std::size_t
  height() const {
    return this->ebiten_texture_.height();
  }
};

}

#endif
