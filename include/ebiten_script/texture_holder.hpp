#ifndef EBITEN_SCRIPT_TEXTURE_HOLDER_HPP
#define EBITEN_SCRIPT_TEXTURE_HOLDER_HPP

#include "ebiten/graphics/texture.hpp"
#include "ebiten/graphics/texture_factory.hpp"
#include "ebiten/image.hpp"
#include "ebiten/image_loader.hpp"
#include "ebiten/noncopyable.hpp"
#include <memory>
#include <string>

namespace ebiten_script {

// TODO: Make a child class of texture_holder?
class texture_holder : private ebiten::noncopyable {
private:
  std::unique_ptr<ebiten::graphics::texture> ebiten_texture_;
  // TODO: I don't wanna use pointers!
  ebiten::graphics::texture* external_ebiten_texture_;
  std::string const path_;
  std::size_t const width_;
  std::size_t const height_;
public:
  texture_holder(std::string const& path)
    : external_ebiten_texture_(nullptr),
      path_(path),
      width_(0),
      height_(0) {
  }
  texture_holder(std::size_t width, std::size_t height)
    : external_ebiten_texture_(nullptr),
      path_(),
      width_(width),
      height_(height) {
  }
  texture_holder(texture_holder&& rhs)
    : ebiten_texture_(std::move(rhs.ebiten_texture_)),
      external_ebiten_texture_(std::move(rhs.external_ebiten_texture_)),
      path_(std::move(rhs.path_)),
      width_(std::move(rhs.width_)),
      height_(std::move(rhs.height_)) {
  }
  void
  instantiate(ebiten::graphics::texture_factory& tf) {
    if (this->is_instantiate()) {
      return;
    }
    if (!this->path_.empty()) {
      std::unique_ptr<ebiten::image const> image =
        ebiten::image_loader::load_png(this->path_);
      this->ebiten_texture_ = tf.from_image(*image);
    } else if (0 < this->width_ && 0 < this->height_) {
      this->ebiten_texture_ = tf.create(this->width_, this->height_);
    }
  }
  bool
  is_instantiate() const {
    return this->external_ebiten_texture_ || this->ebiten_texture_;
  }
  ebiten::graphics::texture&
  ebiten_texture() {
    if (this->external_ebiten_texture_) {
      return *this->external_ebiten_texture_;
    }
    return *this->ebiten_texture_;
  }
  ebiten::graphics::texture const&
  ebiten_texture() const {
    if (this->external_ebiten_texture_) {
      return *this->external_ebiten_texture_;
    }
    return *this->ebiten_texture_;
  }
  void
  set_ebiten_texture(ebiten::graphics::texture* ebiten_texture) {
    this->external_ebiten_texture_ = ebiten_texture;
  }
};

}

#endif
