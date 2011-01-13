#ifndef EBITEN_UTIL_IMAGE_LOADER_HPP
#define EBITEN_UTIL_IMAGE_LOADER_HPP

#include "ebiten/util/image.hpp"
#include "ebiten/util/singleton.hpp"
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_io.hpp>
#include <memory>
#include <vector>

namespace ebiten {
namespace util {

class image_loader : public singleton<image_loader> {
  friend class singleton<image_loader>;
public:
  std::unique_ptr<image>
  load_file(const std::string& filename) {
    boost::gil::rgba8_image_t gil_image;
    boost::gil::png_read_image(filename, gil_image);
    const auto width  = gil_image.width();
    const auto height = gil_image.height();
    std::unique_ptr<std::vector<uint8_t>> pixels(new std::vector<uint8_t>(width * height * 4));
    auto pixelsPtr = reinterpret_cast<boost::gil::rgba8_pixel_t*>(pixels->data());
    auto viewSrc = boost::gil::view(gil_image);
    auto viewDst = boost::gil::interleaved_view(width, height, pixelsPtr, width * 4);
    boost::gil::copy_pixels(viewSrc, viewDst);
    return std::unique_ptr<image>(new image(std::move(pixels), width, height));
  }
private:
  image_loader() {
  }
  ~image_loader() {
  }
};

}
}

#ifdef EBITEN_TEST

namespace ebiten {
namespace util {

TEST(image_loader, load_file) {
  const auto image = image_loader::instance().load_file("test.png");
  EXPECT_EQ(32, image->width());
  EXPECT_EQ(32, image->height());
}

}
}

#endif

#endif

