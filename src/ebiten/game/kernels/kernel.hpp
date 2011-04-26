#ifndef EBITEN_GAME_KERNELS_KERNEL_HPP
#define EBITEN_GAME_KERNELS_KERNEL_HPP

#ifdef EBITEN_MACOSX
#include "ebiten/game/kernels/detail/macosx/kernel.hpp"
#endif

namespace ebiten {
namespace game {
namespace kernels {

typedef detail::kernel kernel;

}
}
}

#endif
