#ifndef SHRIMP_VIEWS_MAP_EDITOR_HPP
#define SHRIMP_VIEWS_MAP_EDITOR_HPP

#include "ebiten/ebiten.hpp"
#include <memory>

namespace shrimp {
namespace views {

class map_editor {
private:
  class gl_canvas_drawer {
  public:
    void
    update(ebiten::graphics::texture_factory&,
           ebiten::input const&) {
    }
    void
    draw(ebiten::graphics::graphics_context& g,
         ebiten::graphics::texture& main_offscreen) {
      g.set_offscreen(main_offscreen);
      g.clear();
    }
  };
  std::unique_ptr<gl_canvas_drawer> gl_canvas_drawer_;
  std::unique_ptr<ebiten::kernel> kernel_;
public:
  template<class GLCanvas>
  map_editor(GLCanvas gl_canvas_)
    : gl_canvas_drawer_(new gl_canvas_drawer()) {
    auto game_update = std::bind(&gl_canvas_drawer::update,
                                 this->gl_canvas_drawer_,
                                 std::placeholders::_1,
                                 std::placeholders::_2);
    auto game_draw = std::bind(&gl_canvas_drawer::draw,
                               this->gl_canvas_drawer_,
                               std::placeholders::_1,
                               std::placeholders::_2);
    this->kernel_.reset(new ebiten::kernel(game_update,
                                           game_draw,
                                           320, 240, 2, 60,
                                           gl_canvas_));
  }
};

}
}

#endif
