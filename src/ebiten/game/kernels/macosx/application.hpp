#ifndef EBITEN_GAME_KERNELS_MACOSX_APPLICATION_HPP
#define EBITEN_GAME_KERNELS_MACOSX_APPLICATION_HPP

#include "ebiten/util/id.hpp"
#include "ebiten/util/singleton.hpp"

namespace ebiten {
namespace game {
namespace kernels {
namespace macosx {

namespace detail {

int run(util::id_ const& native_frame);

}

class application : public util::singleton<application> {
  friend class util::singleton<application>;
public:
  template<class Frame>
  int run(Frame& frame) {
    return detail::run(frame.native_frame());
  }
};

}
}
}
}

#endif
