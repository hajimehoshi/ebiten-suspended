#ifndef EBITEN_GAME_KERNELS_DETAIL_MACOSX_APPLICATION_HPP
#define EBITEN_GAME_KERNELS_DETAIL_MACOSX_APPLICATION_HPP

#include "ebiten/util/id.hpp"
#include <boost/noncopyable.hpp>

namespace ebiten {
namespace game {
namespace kernels {
namespace detail {

int do_run(util::id_ const& native_frame);

class application : private boost::noncopyable {
public:
  template<class Device>
  application(Device& device) {
    do_run(device.frame().native_frame());
  }
};

}
}
}
}

#endif
