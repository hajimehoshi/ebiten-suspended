#ifndef EBITEN_GAME_KERNELS_MACOSX_APPLICATION_HPP
#define EBITEN_GAME_KERNELS_MACOSX_APPLICATION_HPP

#include "ebiten/util/singleton.hpp"
#include <cstdlib>

namespace ebiten {
namespace game {
namespace kernels {
namespace macosx {

class application : public util::singleton<application> {
  friend class util::singleton<application>;
public:
  int run(const std::ptrdiff_t window);
};

}
}
}
}

#endif
