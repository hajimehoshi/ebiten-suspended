#ifndef EBITEN_DETAIL_IOS_RESOURCES_HPP
#define EBITEN_DETAIL_IOS_RESOURCES_HPP

#import <Foundation/Foundation.h>
#include <string>

namespace ebiten {
namespace detail {
namespace {

std::string
get_native_resource_path(std::string const& filename) {
  NSBundle* bundle = [NSBundle mainBundle];
  NSString* ns_path = [bundle pathForResource:[[NSString alloc] initWithUTF8String:filename.c_str()]
                                       ofType:nil];
  if (ns_path != nil) {
    return [ns_path UTF8String];
  }
  return "";
}

}
}
}

#endif
