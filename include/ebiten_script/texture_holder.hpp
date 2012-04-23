#ifndef EBITEN_SCRIPT_TEXTURE_HOLDER_HPP
#define EBITEN_SCRIPT_TEXTURE_HOLDER_HPP

#include "ebiten/graphics/texture.hpp"
#include "ebiten/graphics/texture_factory.hpp"
#include "ebiten/image.hpp"
#include "ebiten/image_loader.hpp"
#include "ebiten/noncopyable.hpp"
#include <cassert>
#include <memory>
#include <string>
#include <iostream>

namespace ebiten_script {

// TODO: Make a child class of texture_holder?
class texture_holder : private ebiten::noncopyable {
private:
  ebiten::graphics::texture ebiten_texture_;
  ebiten::graphics::texture external_ebiten_texture_;
  std::size_t const width_;
  std::size_t const height_;
  std::unique_ptr<ebiten::image const> image_;
public:
  explicit
  texture_holder(std::string const& path)
    : external_ebiten_texture_(),
      width_(0),
      height_(0),
      image_(ebiten::image_loader::load_png(path)) {
  }
  texture_holder(std::size_t width, std::size_t height)
    : external_ebiten_texture_(),
      width_(width),
      height_(height) {
  }
  explicit
  texture_holder(texture_holder&& rhs)
    : ebiten_texture_(std::move(rhs.ebiten_texture_)),
      external_ebiten_texture_(std::move(rhs.external_ebiten_texture_)),
      width_(std::move(rhs.width_)),
      height_(std::move(rhs.height_)),
      image_(std::move(rhs.image_)) {
  }
  void
  instantiate(ebiten::graphics::texture_factory& tf) {
    if (this->is_instantiated()) {
      return;
    }
    if (this->image_) {
      this->ebiten_texture_ = tf.from_image(*this->image_);
      //this->image_.reset();
    } else if (0 < this->width_ && 0 < this->height_) {
      this->ebiten_texture_ = tf.create(this->width_, this->height_);
    }
  }
  bool
  is_instantiated() const {
    return this->external_ebiten_texture_ || this->ebiten_texture_;
  }
  ebiten::graphics::texture&
  ebiten_texture() {
    if (this->external_ebiten_texture_) {
      return this->external_ebiten_texture_;
    }
    assert(static_cast<bool>(this->ebiten_texture_));
    return this->ebiten_texture_;
  }
  ebiten::graphics::texture const&
  ebiten_texture() const {
    if (this->external_ebiten_texture_) {
      return this->external_ebiten_texture_;
    }
    assert(static_cast<bool>(this->ebiten_texture_));
    return this->ebiten_texture_;
  }
  void
  set_ebiten_texture(ebiten::graphics::texture const& ebiten_texture) {
    this->external_ebiten_texture_ = ebiten_texture;
  }
  std::size_t
  width() const {
    if (this->external_ebiten_texture_) {
      return this->external_ebiten_texture_.width();
    }
    if (this->image_) {
      return this->image_->width();
    }
    if (this->width_) {
      return this->width_;
    }
    return 0;
  }
  std::size_t
  height() const {
    if (this->external_ebiten_texture_) {
      return this->external_ebiten_texture_.height();
    }
    if (this->image_) {
      return this->image_->height();
    }
    if (this->height_) {
      return this->height_;
    }
    return 0;
  }
};

}

#endif
