#ifndef EBITEN_TIMERS_TIMER_HPP
#define EBITEN_TIMERS_TIMER_HPP

#if __APPLE__
#include "ebiten/timers/detail/mach/timer.hpp"
#endif

namespace ebiten {
namespace timers {

typedef detail::timer timer;

}
}

#endif
