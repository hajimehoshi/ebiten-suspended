#ifndef EBITEN_KERNELS_DETAIL_MACOSX_APPLICATION_HPP
#define EBITEN_KERNELS_DETAIL_MACOSX_APPLICATION_HPP

#include "ebiten/graphics/view.hpp"

#ifdef EBITEN_MACOSX
#include "ebiten/kernels/detail/macosx/application.mm"
#endif

namespace ebiten {
namespace kernels {
namespace detail {

static void
run_application(graphics::view& view) {
  ::ebiten_kernels_detail_run_application(view);
}

}
}
}

#endif
