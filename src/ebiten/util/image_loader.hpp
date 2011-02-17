#ifndef EBITEN_UTIL_IMAGE_LOADER_HPP
#define EBITEN_UTIL_IMAGE_LOADER_HPP

#include "ebiten/util/image.hpp"
#include "ebiten/util/singleton.hpp"
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_io.hpp>
#include <boost/optional.hpp>
#include <boost/utility/in_place_factory.hpp>

namespace ebiten {
namespace util {

class image_loader : public singleton<image_loader> {
  friend class singleton<image_loader>;
public:
  void
  load_file(boost::optional<image>& image, std::string const& filename) {
    typedef boost::gil::rgba8_image_t gil_image_t;
    gil_image_t gil_image;
    boost::gil::png_read_image(filename, gil_image);
    std::size_t const width  = gil_image.width();
    std::size_t const height = gil_image.height();
    image = boost::in_place(width, height);
    boost::gil::rgba8_pixel_t* pixelsPtr =
      reinterpret_cast<boost::gil::rgba8_pixel_t*>(image->pixels().data());
    gil_image_t::view_t viewSrc =
      boost::gil::view(gil_image);
    gil_image_t::view_t viewDst =
      boost::gil::interleaved_view(width, height, pixelsPtr, width * 4);
    boost::gil::copy_pixels(viewSrc, viewDst);
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
  boost::optional<image> image;
  image_loader::instance().load_file(image, "test.png");
  EXPECT_TRUE(image);
  EXPECT_EQ(32u, image->width());
  EXPECT_EQ(32u, image->height());
}

}
}

#endif

#endif

