#ifndef EBITEN_GAME_KERNELS_DETAIL_MACOSX_KERNEL_HPP
#define EBITEN_GAME_KERNELS_DETAIL_MACOSX_KERNEL_HPP

#include "ebiten/game/kernels/detail/kernel.hpp"

#include "ebiten/game/frames/frame.hpp"
#include "ebiten/game/graphics/opengl/device.hpp"
#include "ebiten/game/graphics/opengl/cocoa/view.hpp"
#include "ebiten/game/kernels/detail/macosx/application.hpp"
#include "ebiten/game/timers/timer.hpp"

namespace ebiten {
namespace game {
namespace kernels {
namespace detail {

typedef detail::kernel_<
  graphics::opengl::device<graphics::opengl::cocoa::view<frames::frame> >,
  timers::timer,
  kernels::detail::macosx::application> kernel;

}
}
}
}

#endif
