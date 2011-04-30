#ifndef EBITEN_GAME_KERNELS_KERNEL_HPP
#define EBITEN_GAME_KERNELS_KERNEL_HPP

#include "ebiten/game/kernels/detail/kernel.hpp"
#include "ebiten/game/graphics/device.hpp"
#include "ebiten/game/timers/timer.hpp"

namespace ebiten {
namespace game {
namespace kernels {

typedef detail::kernel<
  graphics::device,
  timers::timer> kernel;

}
}
}

#endif
