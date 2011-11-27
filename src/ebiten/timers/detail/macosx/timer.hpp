#ifndef EBITEN_TIMERS_DETAIL_MACOSX_TIMER_HPP
#define EBITEN_TIMERS_DETAIL_MACOSX_TIMER_HPP

#include <boost/noncopyable.hpp>
#include <mach/mach_time.h>
#include <cassert>
#include <cstddef>

namespace ebiten {
namespace timers {
namespace detail {

class timer : private boost::noncopyable {
private:
  uint64_t const duration_per_frame_;
  uint64_t next_time_to_update_;
public:
  explicit
  timer(std::size_t fps)
    : duration_per_frame_(calc_duration_per_frame(fps)),
      next_time_to_update_(mach_absolute_time()) {
  }
  void
  wait_frame() {
    uint64_t const now = mach_absolute_time();
    if (now < this->next_time_to_update_) {
      mach_wait_until(this->next_time_to_update_);
    }
    this->next_time_to_update_ += this->duration_per_frame_;
  }
private:
  static uint64_t
  calc_duration_per_frame(std::size_t fps) {
    assert(fps);
    mach_timebase_info_data_t timebase_info;
    mach_timebase_info(&timebase_info);
    return 1000 * 1000 * 1000 * timebase_info.denom / fps / timebase_info.numer;
  }
};

}
}
}

#endif
