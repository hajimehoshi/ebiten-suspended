#ifndef SHRIMP_VIEWS_DETAIL_WX_GL_CANVAS_HPP
#define SHRIMP_VIEWS_DETAIL_WX_GL_CANVAS_HPP

#include "shrimp/views/detail/wx/wx.hpp"
#include "ebiten/ebiten.hpp"
#include <functional>
#include <memory>

namespace shrimp {
namespace views {
namespace detail {

class gl_canvas : public wxGLCanvas {
private:
  std::unique_ptr<ebiten::kernel> ebiten_kernel_;
  int x_ = 0;
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
      ebiten_kernel_(new ebiten::kernel(update_func,
                                        draw_func,
                                        wxDefaultSize.GetWidth(),
                                        wxDefaultSize.GetHeight(),
                                        1, 60,
                                        this)) {
  }
  void
  on_idle(wxIdleEvent&) {
    this->Refresh(false);
  }
  /*void
  on_paint(wxPaintEvent&) {
    this->draw();
    }*/
private:
  /*void
  draw() {
    // TODO: Use ebiten!
    wxPaintDC dc(this);
    this->x_ += 1;
    this->x_ %= 256;
    ::glClearColor(this->x_ / 255.0, 0.5, 1, 0);
    ::glClear(GL_COLOR_BUFFER_BIT);
    ::glFlush();
    this->SwapBuffers();
    }*/
private:
  wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(gl_canvas, wxGLCanvas)
EVT_IDLE(gl_canvas::on_idle)
//EVT_PAINT(gl_canvas::on_paint)
wxEND_EVENT_TABLE()

}
}
}

#endif
