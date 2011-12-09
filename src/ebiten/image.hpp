#ifndef EBITEN_IMAGE_HPP
#define EBITEN_IMAGE_HPP

#include "ebiten/noncopyable.hpp"
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
      pixels_(width * height * 4) {
  }
  template<class Loader>
  image(Loader const& loader, std::string const& filename) {
    loader(filename, this->width_, this->height_, this->pixels_);
  }
  std::vector<uint8_t>&
  pixels() {
    return this->pixels_;
  }
  std::vector<uint8_t> const&
  pixels() const {
    return this->pixels_;
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