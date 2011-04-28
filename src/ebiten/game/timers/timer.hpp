#ifndef EBITEN_GAME_TIMERS_TIMER_HPP
#define EBITEN_GAME_TIMERS_TIMER_HPP

#include "ebiten/platform.hpp"

#ifdef EBITEN_MACOSX
#include "ebiten/game/timers/detail/macosx/timer.hpp"
#endif

namespace ebiten {
namespace game {
namespace timers {

typedef detail::timer timer;

}
}
}

#endif
