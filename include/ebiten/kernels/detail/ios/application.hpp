#ifndef EBITEN_KERNELS_DETAIL_IOS_APPLICATION_HPP
#define EBITEN_KERNELS_DETAIL_IOS_APPLICATION_HPP

#include "ebiten/frames/native_frame.hpp"
#include "ebiten/graphics/view.hpp"
#include "ebiten/noncopyable.hpp"

namespace ebiten {
namespace kernels {
namespace detail {

class application : private noncopyable {
public:
  static void
  run(frames::native_frame native_frame) {
    // TODO: Implement?
  }
};

}
}
}

#endif
