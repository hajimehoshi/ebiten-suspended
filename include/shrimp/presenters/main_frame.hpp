#ifndef SHRIMP_PRESENTERS_PRESENTER_HPP
#define SHRIMP_PRESENTERS_PRESENTER_HPP

#include "shrimp/views/main_frame.hpp"
#include "ebiten/noncopyable.hpp"

namespace shrimp {
namespace presenters {

class main_frame : private ebiten::noncopyable {
private:
  views::main_frame& main_frame_;
  enum {
    EDITOR_MAP,
  } editor_state_;
public:
  main_frame(views::main_frame& main_frame)
    : main_frame_(main_frame),
      editor_state_(EDITOR_MAP) {
    this->refresh_view();
  }
private:
  void
  refresh_view() {
    
  }
};

}
}

#endif
