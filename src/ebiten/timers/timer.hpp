#ifndef EBITEN_TIMERS_TIMER_HPP
#define EBITEN_TIMERS_TIMER_HPP

#include "ebiten/platform.hpp"

#ifdef EBITEN_MACOSX
#include "ebiten/timers/detail/macosx/timer.hpp"
#endif

namespace ebiten {
namespace timers {

typedef detail::timer timer;

}
}

#endif
