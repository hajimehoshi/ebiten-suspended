#ifndef SHRIMP_VIEWS_DETIAL_WX_MAP_EDITOR_HPP
#define SHRIMP_VIEWS_DETAIL_WX_MAP_EDITOR_HPP

#include "shrimp/views/detail/wx/gl_canvas.hpp"
#include "ebiten/ebiten.hpp"
#include <functional>
#include <memory>

namespace shrimp {
namespace views {
namespace detail {

class map_editor {
private:
  class gl_canvas_drawer {
  public:
    bool
    update(ebiten::graphics::texture_factory&,
           ebiten::input const&) {
      return true;
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
  map_editor(gl_canvas* gl_canvas_)
    : gl_canvas_drawer_(new gl_canvas_drawer()) {
  }
};

}
}
}

#endif
