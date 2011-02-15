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
  std::size_t const width_;
  std::size_t const height_;
  std::vector<uint8_t> pixels_;
public:
  image(std::size_t width,
        std::size_t height)
    : width_(width),
      height_(height),
      pixels_(width * height * 4) {
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
}

#endif
