#ifndef EBITEN_DETAIL_COCOA_IMAGE_LOADER_HPP
#define EBITEN_DETAIL_IMAGE_LOADER_HPP

#include "ebiten/image.hpp"
#import <Cocoa/Cocoa.h>
#include <algorithm>
#include <memory>

namespace ebiten {
namespace detail {

class image_loader {
public:
  image_loader() = delete;
  image_loader(image_loader const&) = delete;
  image_loader& operator=(image_loader const&) = delete;
  static std::unique_ptr<image>
  load(std::string const& filename) {
    std::unique_ptr<image> image;
    @autoreleasepool {
      NSString* ns_filename = [NSString stringWithCString:filename.c_str()
                                                 encoding:NSUTF8StringEncoding];
      NSImage* ns_image = [[NSImage alloc] initWithContentsOfFile: ns_filename];
      NSBitmapImageRep* ns_bitmap = [[ns_image representations] objectAtIndex:0];
      std::size_t const width  = [ns_bitmap pixelsWide];
      std::size_t const height = [ns_bitmap pixelsHigh];
      uint8_t* src_pixels      = [ns_bitmap bitmapData];
      image.reset(new ebiten::image(width, height));
      auto& dst_pixels = image->pixels();
      std::copy(src_pixels, src_pixels + width * height * 4,
                std::begin(dst_pixels));
    }
    return image;
  }
};

}
}

#endif
