#ifndef SHRIMP_MODELS_MAP_HPP
#define SHRIMP_MODELS_MAP_HPP

#include "ebiten/noncopyable.hpp"
#include <string>

namespace shrimp {
namespace models {

class map : private ebiten::noncopyable {
private:
  std::size_t width_;
  std::size_t height_;
public:
  map() {
  }
  std::size_t
  width() const {
    return this->width_;
  }
  void
  set_width(std::size_t width) {
    this->width_ = width;
  }
  std::size_t
  height() const {
    return this->height_;
  }
  void
  set_height(std::size_t height) {
    this->height_ = height;
  }
};

}
}

#endif
