#ifndef EBITEN_INPUT_HPP
#define EBITEN_INPUT_HPP

#include "ebiten/noncopyable.hpp"
#include <tuple>

namespace ebiten {

class input : private noncopyable {
private:
  int touch_x_;
  int touch_y_;
  bool is_touched_;
public:
  input()
    : touch_x_(-1),
      touch_y_(-1),
      is_touched_(false) {
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
  set_touches_location(int, int x, int y) {
    NSLog(@"Set Touches Location: %d, %d", x, y);
    this->touch_x_ = x;
    this->touch_y_ = y;
  }
  void
  set_touched(int, bool is_touched) {
    NSLog(@"Set Touched: %@", is_touched ? @"True" : @"False");
    this->is_touched_ = is_touched;
  }
};

}

#endif
