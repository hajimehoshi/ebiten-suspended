#ifndef EBITEN_IMAGE_LOADER_HPP
#define EBITEN_IMAGE_LOADER_HPP

#include "ebiten/platform.hpp"

#if defined(EBITEN_MACOSX) || defined(EBITEN_IOS)
# include "ebiten/detail/cocoa/image_loader.hpp"
#endif

namespace ebiten {

typedef detail::image_loader image_loader;

}

/*#ifdef EBITEN_TEST

#include "ebiten/image.hpp"

namespace ebiten {

BOOST_AUTO_TEST_CASE(image_loader_png_image_loader) {
  image img(png_image_loader, "test.png");
  BOOST_CHECK_EQUAL(32u, img.width());
  BOOST_CHECK_EQUAL(32u, img.height());
}

}

#endif*/

#endif
