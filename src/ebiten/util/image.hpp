#ifndef EBITEN_UTIL_IMAGE_HPP
#define EBITEN_UTIL_IMAGE_HPP

#include <boost/noncopyable.hpp>
#include <memory>
#include <vector>

namespace ebiten {
namespace util {

class image : private boost::noncopyable {
private:
  const std::unique_ptr<std::vector<uint8_t>> pixels_;
  const std::size_t width_;
  const std::size_t height_;
public:
  image(std::unique_ptr<std::vector<uint8_t> > pixels_, std::size_t width_, std::size_t height_)
    : pixels_(std::move(pixels_)),
      width_(width_),
      height_(height_) {
    assert(this->pixels_->size() == width_ * height_ * 4);
  }
  const std::vector<uint8_t>&
  pixels() const {
    assert(this->pixels_);
    return *this->pixels_;
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
}

#endif
