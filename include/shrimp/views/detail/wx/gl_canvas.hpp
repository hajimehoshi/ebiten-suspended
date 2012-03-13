#ifndef SHRIMP_VIEWS_DETAIL_WX_GL_CANVAS_HPP
#define SHRIMP_VIEWS_DETAIL_WX_GL_CANVAS_HPP

#include "shrimp/views/detail/wx/wx.hpp"
#include "ebiten/ebiten.hpp"
#include <functional>

namespace shrimp {
namespace views {
namespace detail {

class gl_canvas : public wxGLCanvas {
private:
  ebiten::kernel ebiten_kernel_;
public:
  gl_canvas(wxWindow* parent,
            std::function<bool(ebiten::graphics::texture_factory&,
                               ebiten::input const&)> const& update_func,
            std::function<void(ebiten::graphics::graphics_context&,
                               ebiten::graphics::texture&)> const& draw_func)
    : wxGLCanvas(parent,
                 wxID_ANY,
                 nullptr,
                 wxDefaultPosition,
                 wxDefaultSize,
                 wxFULL_REPAINT_ON_RESIZE,
                 wxGLCanvasName,
                 wxNullPalette),
      ebiten_kernel_(update_func,
                     draw_func,
                     320,
                     240,
                     1, 60,
                     this) {
  }
  void
  on_idle(wxIdleEvent&) {
    this->Refresh(false);
  }
  void
  on_size(wxSizeEvent& e) {
    wxSize const& size = e.GetSize();
    this->ebiten_kernel_.set_screen_size(size.GetWidth(), size.GetHeight());
  }
private:
  wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(gl_canvas, wxGLCanvas)
EVT_IDLE(gl_canvas::on_idle)
EVT_SIZE(gl_canvas::on_size)
wxEND_EVENT_TABLE()

}
}
}

#endif
