#ifndef EBITEN_IMAGE_LOADER_HPP
#define EBITEN_IMAGE_LOADER_HPP

#include "ebiten/platform.hpp"

#ifdef EBITEN_MACOSX
# include "ebiten/detail/macosx/image_loader.hpp"
#endif

#ifdef EBITEN_IOS
# include "ebiten/detail/ios/image_loader.hpp"
#endif

namespace ebiten {

typedef detail::image_loader image_loader;

}

#ifdef EBITEN_TEST

#include "ebiten/image.hpp"

namespace ebiten {

TEST(image_loader, load_png) {
  std::unique_ptr<image> img = image_loader::load_png("samples/resources/test.png");
  EXPECT_EQ(32u, img->width());
  EXPECT_EQ(32u, img->height());
}

TEST(image_loader, load_invalid_file) {
  EXPECT_THROW((image_loader::load_png("foo")), std::runtime_error);
}

}

#endif

#endif
