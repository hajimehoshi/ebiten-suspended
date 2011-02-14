#ifndef EBITEN_UTIL_IMAGE_HPP
#define EBITEN_UTIL_IMAGE_HPP

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <memory>
#include <vector>

namespace ebiten {
namespace util {

class image : private boost::noncopyable {
private:
  const boost::shared_ptr<std::vector<uint8_t> > pixels_;
  const std::size_t width_;
  const std::size_t height_;
public:
  image(boost::shared_ptr<std::vector<uint8_t> > pixels, std::size_t width, std::size_t height)
    : pixels_(pixels),
      width_(width),
      height_(height) {
    assert(this->pixels_->size() == width * height * 4);
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
