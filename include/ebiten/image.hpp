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
  image(image const& src,
        std::size_t width_with_padding,
        std::size_t height_with_padding)
    : width_(width_with_padding),
      height_(height_with_padding),
      pixels_(width_with_padding * height_with_padding * 4, 0) {
    std::vector<uint8_t> const& src_pixels = src.pixels();
    std::size_t const src_width  = src.width();
    std::size_t const src_height = src.height();
    for (std::size_t j = 0; j < src_height; ++j) {
      std::copy(src_pixels.begin() + j       * src_width * 4,
                src_pixels.begin() + (j + 1) * src_width * 4,
                this->pixels_.begin() + j * width_with_padding * 4);
    }
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

#ifdef EBITEN_TEST

#include "ebiten/image_loader.hpp"

namespace ebiten {

TEST(image, pixels) {
  {
    std::unique_ptr<image> img = image_loader::load_png("samples/resources/test.png");
    EXPECT_EQ(32u, img->width());
    EXPECT_EQ(32u, img->height());
    EXPECT_EQ(32u * 32u * 4, img->pixels().size());
  }
  {
    std::unique_ptr<image> img = image_loader::load_png("samples/resources/blocks.png");
    EXPECT_EQ(70u, img->width());
    EXPECT_EQ(10u, img->height());
    EXPECT_EQ(70u * 10u * 4, img->pixels().size());
  }
}

}

#endif

#endif
