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
  std::unique_ptr<image> img = image_loader::load_png("test.png");
  EXPECT_EQ(32u, img->width());
  EXPECT_EQ(32u, img->height());
}

}

#endif

#endif
