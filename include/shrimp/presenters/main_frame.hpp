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
  ebiten::graphics::texture_pointer transparent_tile_;
  bool transparent_tile_rendered_;
public:
  main_frame(views::main_frame& main_frame)
    : view_(main_frame),
      source_(SOURCE_MAP),
      map_editor_scroll_x_(0),
      map_editor_scroll_y_(0),
      map_editor_scale_(1),
      map_width_(20),
      map_height_(15),
      transparent_tile_rendered_(false) {
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
  on_update_map_editor(ebiten::graphics::texture_factory& tf,
                       ebiten::input const&) {
    if (!this->transparent_tile_) {
      this->transparent_tile_ = tf.create(16, 16);
    }
  }
  void
  on_draw_map_editor(ebiten::graphics::graphics_context& g,
                     ebiten::graphics::texture& offscreen) {
    if (this->transparent_tile_ && !this->transparent_tile_rendered_) {
      g.set_offscreen(this->transparent_tile_);
      g.draw_rect(0, 0, 8, 8,
                  255, 255, 255, 255);
      g.draw_rect(8, 0, 8, 8,
                  204, 204, 204, 255);
      g.draw_rect(0, 8, 8, 8,
                  204, 204, 204, 255);
      g.draw_rect(8, 8, 8, 8,
                  255, 255, 255, 255);
      this->transparent_tile_rendered_ = true;
    }
    g.set_offscreen(offscreen);
    g.clear();
    g.fill(128, 128, 128, 255);
    for (std::size_t j = 0; j < this->map_height_; ++j) {
      for (std::size_t i = 0; i < this->map_width_; ++i) {
        if (!this->transparent_tile_) {
          continue;
        }
        std::size_t const grid_size = this->map_editor_scale_ * 16;
        int const x = static_cast<int>(this->map_editor_scroll_x_ + grid_size * i);
        int const y = static_cast<int>(this->map_editor_scroll_y_ + grid_size * j);
        ebiten::graphics::geometry_matrix geom_mat;
        geom_mat.set_a(this->map_editor_scale_);
        geom_mat.set_d(this->map_editor_scale_);
        geom_mat.set_tx(x);
        geom_mat.set_ty(y);
        ebiten::graphics::color_matrix const& color_mat =
          ebiten::graphics::color_matrix::identity();
        g.draw_texture(this->transparent_tile_,
                       0, 0, 16, 16,
                       geom_mat, color_mat);
      }
    }
  }
};

}
}

#endif
