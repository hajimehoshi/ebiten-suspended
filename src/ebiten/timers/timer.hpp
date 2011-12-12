#ifndef EBITEN_TIMERS_TIMER_HPP
#define EBITEN_TIMERS_TIMER_HPP

#include "ebiten/platform.hpp"

#if defined(EBITEN_MACOSX) || defined(EBITEN_IOS)
#include "ebiten/timers/detail/mach/timer.hpp"
#endif

namespace ebiten {
namespace timers {

typedef detail::timer timer;

}
}

#endif
