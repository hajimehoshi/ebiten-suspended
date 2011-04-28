#ifndef EBITEN_GAME_KERNELS_KERNEL_HPP
#define EBITEN_GAME_KERNELS_KERNEL_HPP

#include "ebiten/platform.hpp"

#ifdef EBITEN_MACOSX 
#include "ebiten/game/kernels/detail/macosx/application.hpp"
#endif

#include "ebiten/game/kernels/detail/kernel.hpp"
#include "ebiten/game/frames/frame.hpp"
#include "ebiten/game/graphics/device.hpp"
#include "ebiten/game/timers/timer.hpp"

namespace ebiten {
namespace game {
namespace kernels {

typedef detail::kernel<
  graphics::device<frames::frame>::type,
  timers::timer,
  kernels::detail::application> kernel;

}
}
}

#endif
