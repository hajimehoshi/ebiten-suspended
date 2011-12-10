#ifndef EBITEN_KERNELS_DETAIL_MACOSX_APPLICATION_HPP
#define EBITEN_KERNELS_DETAIL_MACOSX_APPLICATION_HPP

#include "ebiten/frames/frame.hpp"
#include "ebiten/kernels/detail/macosx/application.mm"

namespace ebiten {
namespace kernels {
namespace detail {

void
run_application(frames::frame& frame) {
  ::ebiten_kernels_detail_run_application(frame);
}

}
}
}

#endif
