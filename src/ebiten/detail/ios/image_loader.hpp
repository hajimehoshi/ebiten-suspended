#ifndef EBITEN_DETAIL_IOS_IMAGE_LOADER_HPP
#define EBITEN_DETAIL_IOS_IMAGE_LOADER_HPP

#include "ebiten/image.hpp"
#import <UIKit/UIKit.h>
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
      NSData* ns_data = [NSData dataWithContentsOfFile:ns_filename];
      CGDataProviderRef cg_data_provider =
        ::CGDataProviderCreateWithCFData((__bridge CFDataRef)ns_data);
      CGImageRef cg_image =
        ::CGImageCreateWithPNGDataProvider(cg_data_provider, NULL, true, kCGRenderingIntentDefault);
      CFDataRef cf_data = ::CGDataProviderCopyData(CGImageGetDataProvider(cg_image));

      std::size_t const width   = ::CGImageGetWidth(cg_image);
      std::size_t const height  = ::CGImageGetHeight(cg_image);
      uint8_t const* src_pixels = ::CFDataGetBytePtr(cf_data);
      image.reset(new ebiten::image(width, height));
      auto& dst_pixels = image->pixels();
      std::copy(src_pixels, src_pixels + width * height * 4,
      std::begin(dst_pixels));

      ::CFRelease(cf_data);
      ::CGImageRelease(cg_image);
      ::CGDataProviderRelease(cg_data_provider);
    }
    return image;
  }
};

}
}

#endif
