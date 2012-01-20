#ifndef EBITEN_INPUT_HPP
#define EBITEN_INPUT_HPP

#include "ebiten/noncopyable.hpp"
#include <cassert>
#include <tuple>

namespace ebiten {

class input : private noncopyable {
private:
  std::size_t screen_scale_;
  int touch_x_;
  int touch_y_;
  bool is_touched_;
public:
  input(std::size_t screen_scale)
    : screen_scale_(screen_scale),
      touch_x_(-1),
      touch_y_(-1),
      is_touched_(false) {
    assert(this->screen_scale_);
  }
  std::tuple<int, int>
  touches(int) const {
    return std::tuple<int, int>(this->touch_x_, this->touch_y_);
  }
  bool
  is_touched(int) const {
    return this->is_touched_;
  }
  // TODO: Threading?
  void
  set_touches_real_location(int, int x, int y) {
    this->touch_x_ = static_cast<int>(x / this->screen_scale_);
    this->touch_y_ = static_cast<int>(y / this->screen_scale_);
  }
  void
  set_touched(int, bool is_touched) {
    this->is_touched_ = is_touched;
  }
};

}

#endif
