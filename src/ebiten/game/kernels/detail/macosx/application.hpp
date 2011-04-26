#ifndef EBITEN_GAME_KERNELS_DETAIL_MACOSX_APPLICATION_HPP
#define EBITEN_GAME_KERNELS_DETAIL_MACOSX_APPLICATION_HPP

#include "ebiten/game/kernels/detail/kernel.hpp"
#include "ebiten/util/id.hpp"
#include <boost/noncopyable.hpp>

namespace ebiten {
namespace game {
namespace kernels {
namespace detail {

int do_run(util::id_ const& native_frame);

class application : private boost::noncopyable {
  template<class Device, class Timer, class Application>
  friend class kernels::detail::kernel;
private:
  application() {
  }
public:
  template<class Device>
  int run(Device& device) {
    return do_run(device.frame().native_frame());
  }
};

}
}
}
}

#endif
