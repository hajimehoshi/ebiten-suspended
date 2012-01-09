#ifndef EBITEN_RESOURCES_HPP
#define EBITEN_RESOURCES_HPP

#include "ebiten/platform.hpp"

#ifdef EBITEN_MACOSX
# include "ebiten/detail/macosx/resources.hpp"
#endif

#ifdef EBITEN_IOS
# include "ebiten/detail/ios/resources.hpp"
#endif

#include <fstream>
#include <string>

namespace ebiten {
namespace {

std::string
get_resource_path(std::string const& filename) {
  std::string const& resource_path = detail::get_native_resource_path(filename);
  if (!resource_path.empty()) {
    return resource_path;
  }
  if (std::ifstream(filename)) {
    return filename;
  }
  return "";
}

}
}

#endif
