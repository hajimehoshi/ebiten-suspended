#ifndef EBITEN_GAME_KERNELS_MACOSX_APPLICATION_HPP
#define EBITEN_GAME_KERNELS_MACOSX_APPLICATION_HPP

#include "ebiten/game/kernels/base_kernel.hpp"
#include "ebiten/util/id.hpp"
#include <boost/noncopyable.hpp>

namespace ebiten {
namespace game {
namespace kernels {
namespace macosx {

namespace detail {

int run(util::id_ const& native_frame);

}

class application : private boost::noncopyable {
  template<class Device, class Timer, class Application>
  friend class kernels::base_kernel;
private:
  application() {
  }
public:
  template<class Device>
  int run(Device& device) {
    return detail::run(device->frame().native_frame());
  }
};

}
}
}
}

#endif
