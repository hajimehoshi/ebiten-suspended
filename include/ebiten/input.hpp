#ifndef EBITEN_INPUT_HPP
#define EBITEN_INPUT_HPP

/*#include "ebiten/platform.hpp"

#ifdef EBITEN_MACOSX
# include "ebiten/detail/macosx/input.hpp"
#endif*/

#include "ebiten/graphics/native_view.hpp"
#include "ebiten/noncopyable.hpp"
#include <tuple>

namespace ebiten {

class input : private noncopyable {
private:
  int touch_x_;
  int touch_y_;
  bool is_touched_;
public:
  input(graphics::native_view native_view)
    : touch_x_(-1),
      touch_y_(-1),
      is_touched_(false) {
    graphics::detail::set_input_funcs(native_view,
                                      std::bind(&input::set_touches_location,
                                                this,
                                                std::placeholders::_1,
                                                std::placeholders::_2,
                                                std::placeholders::_3),
                                      std::bind(&input::set_touched,
                                                this,
                                                std::placeholders::_1,
                                                std::placeholders::_2));
  }
  std::tuple<int, int>
  touches(int) const {
    return std::tuple<int, int>(this->touch_x_, this->touch_y_);
  }
  bool
  is_touched(int) const {
    return this->is_touched_;
  }
private:
  // TODO: Threading?
  // TODO: Integrate thease functions?
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
