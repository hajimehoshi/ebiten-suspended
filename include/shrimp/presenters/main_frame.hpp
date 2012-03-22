#ifndef SHRIMP_PRESENTERS_MAIN_FRAME_HPP
#define SHRIMP_PRESENTERS_MAIN_FRAME_HPP

#include "shrimp/views/main_frame.hpp"
#include "ebiten/ebiten.hpp"
#include "ebiten/noncopyable.hpp"

namespace shrimp {
namespace presenters {

class main_frame : private ebiten::noncopyable {
private:
  views::main_frame& view_;
  enum source_type {
    SOURCE_MAP,
  } source_;
public:
  main_frame(views::main_frame& main_frame)
    : view_(main_frame),
      source_(SOURCE_MAP) {
    this->refresh_view();
  }
private:
  void
  refresh_view() {
    switch (this->source_) {
    case SOURCE_MAP:
      this->view_.set_source_map();
      break;
    default:
      break;
    }
  }
  void
  on_draw_map_editor(ebiten::graphics::graphics_context& g,
                     ebiten::graphics::texture&) {
    g.clear();
    g.fill(0, 128, 255, 255);
    g.draw_rect(20, 40, 500, 100,
                0, 255, 255, 255);
  }
};

}
}

#endif
