#ifndef EBITEN_GAME_KERNELS_MACOSX_KERNEL_HPP
#define EBITEN_GAME_KERNELS_MACOSX_KERNEL_HPP

#include "ebiten/game/kernels/kernel.hpp"

#include "ebiten/game/frames/cocoa/frame.hpp"
#include "ebiten/game/graphics/opengl/device.hpp"
#include "ebiten/game/graphics/opengl/cocoa/view.hpp"
#include "ebiten/game/kernels/macosx/application.hpp"
#include "ebiten/game/timers/mach/timer.hpp"

namespace ebiten {
namespace game {
namespace kernels {
namespace macosx {

typedef kernels::kernel<
  graphics::opengl::device,
  frames::cocoa::frame,
  graphics::opengl::cocoa::view<frames::cocoa::frame>,
  timers::mach::timer,
  kernels::macosx::application> kernel;

}
}
}
}

#endif
