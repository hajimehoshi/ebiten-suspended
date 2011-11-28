#ifndef EBITEN_KERNEL_DETAIL_MACOSX_APPLICATION_HPP
#define EBITEN_KERNEL_DETAIL_MACOSX_APPLICATION_HPP

#include "ebiten/frames/frame.hpp"
#include "ebiten/kernel/detail/macosx/application.mm"

namespace ebiten {
namespace kernel {
namespace detail {

void
run_application(frames::frame& frame) {
  ::ebiten_kernel_detail_run_application(frame);
}

}
}
}

#endif
