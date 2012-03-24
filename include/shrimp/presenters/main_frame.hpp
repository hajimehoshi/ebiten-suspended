#ifndef SHRIMP_PRESENTERS_MAIN_FRAME_HPP
#define SHRIMP_PRESENTERS_MAIN_FRAME_HPP

#include "shrimp/views/main_frame.hpp"
#include "ebiten/ebiten.hpp"
#include "ebiten/noncopyable.hpp"
#include <functional>

namespace shrimp {
namespace presenters {

class main_frame : private ebiten::noncopyable {
private:
  views::main_frame& view_;
  enum source_type {
    SOURCE_MAP,
  } source_;
  int map_editor_scroll_x_;
  int map_editor_scroll_y_;
  std::size_t map_editor_scale_;
  std::size_t map_width_;
  std::size_t map_height_;
public:
  main_frame(views::main_frame& main_frame)
    : view_(main_frame),
      source_(SOURCE_MAP),
      map_editor_scroll_x_(0),
      map_editor_scroll_y_(0),
      map_editor_scale_(1),
      map_width_(20),
      map_height_(15) {
    this->view_.set_update_map_editor_func(std::bind(&main_frame::on_update_map_editor,
                                                     this,
                                                     std::placeholders::_1,
                                                     std::placeholders::_2));
    this->view_.set_draw_map_editor_func(std::bind(&main_frame::on_draw_map_editor,
                                                   this,
                                                   std::placeholders::_1,
                                                   std::placeholders::_2));
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
  on_update_map_editor(ebiten::graphics::texture_factory&,
                       ebiten::input const&) {
  }
  void
  on_draw_map_editor(ebiten::graphics::graphics_context& g,
                     ebiten::graphics::texture&) {
    g.clear();
    g.fill(128, 128, 128, 255);
    int const x = this->map_editor_scroll_x_;
    int const y = this->map_editor_scroll_y_;
    std::size_t const width  = this->map_editor_scale_ * 16 * this->map_width_;
    std::size_t const height = this->map_editor_scale_ * 16 * this->map_height_;
    g.draw_rect(x, y, width, height,
                255, 255, 255, 255);
  }
};

}
}

#endif
