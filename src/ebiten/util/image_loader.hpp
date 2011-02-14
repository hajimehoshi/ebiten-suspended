#ifndef EBITEN_UTIL_IMAGE_LOADER_HPP
#define EBITEN_UTIL_IMAGE_LOADER_HPP

#include "ebiten/util/image.hpp"
#include "ebiten/util/singleton.hpp"
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_io.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

namespace ebiten {
namespace util {

class image_loader : public singleton<image_loader> {
  friend class singleton<image_loader>;
public:
  boost::shared_ptr<image>
  load_file(std::string const& filename) {
    typedef boost::gil::rgba8_image_t gil_image_t;
    gil_image_t gil_image;
    boost::gil::png_read_image(filename, gil_image);
    std::size_t const width  = gil_image.width();
    std::size_t const height = gil_image.height();
    boost::shared_ptr<std::vector<uint8_t> > pixels =
      boost::make_shared<std::vector<uint8_t> >(width * height * 4);
    boost::gil::rgba8_pixel_t* pixelsPtr =
      reinterpret_cast<boost::gil::rgba8_pixel_t*>(pixels->data());
    gil_image_t::view_t viewSrc = boost::gil::view(gil_image);
    gil_image_t::view_t viewDst = boost::gil::interleaved_view(width, height, pixelsPtr, width * 4);
    boost::gil::copy_pixels(viewSrc, viewDst);
    return boost::make_shared<image>(pixels, width, height);
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
  boost::shared_ptr<image> image = image_loader::instance().load_file("test.png");
  EXPECT_EQ(32u, image->width());
  EXPECT_EQ(32u, image->height());
}

}
}

#endif

#endif

