#ifndef EBITEN_IMAGE_HPP
#define EBITEN_IMAGE_HPP

#include "ebiten/noncopyable.hpp"
#include <algorithm>
#include <vector>

namespace ebiten {

class image : private noncopyable {
private:
  std::size_t width_;
  std::size_t height_;
  std::vector<uint8_t> pixels_;
public:
  image(std::size_t width,
        std::size_t height)
    : width_(width),
      height_(height),
      pixels_(width * height * 4, 0) {
  }
  std::vector<uint8_t>&
  pixels() {
    return this->pixels_;
  }
  std::vector<uint8_t> const&
  pixels() const {
    return this->pixels_;
  }
  void
  pixels_with_padding(std::vector<uint8_t>& values,
                      std::size_t width,
                      std::size_t height) const {
    assert(values.size() == width * height * 4);
    assert(this->width_  <= width);
    assert(this->height_ <= height);
    values.clear();
    for (std::size_t j = 0; j < this->height_; ++j) {
      std::copy(this->pixels_.begin() + j       * this->width_ * 4,
                this->pixels_.begin() + (j + 1) * this->width_ * 4,
                values.begin() + j * width * 4);
    }
  }
  std::size_t
  width() const {
    return this->width_;
  }
  std::size_t
  height() const {
    return this->height_;
  }
};

}

#endif
