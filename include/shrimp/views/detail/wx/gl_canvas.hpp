#ifndef SHRIMP_VIEWS_DETAIL_WX_GL_CANVS_HPP
#define SHRIMP_VIEWS_DETAIL_WX_GL_CANVS_HPP

#include "shrimp/views/detail/wx/wx.hpp"

namespace shrimp {
namespace views {
namespace detail {

class gl_canvas : public wxGLCanvas {
private:
  wxGLContext* context_;
  int x_ = 0;
public:
  gl_canvas(wxWindow* parent)
    : wxGLCanvas(parent,
                 wxID_ANY,
                 nullptr,
                 wxDefaultPosition,
                 wxDefaultSize,
                 wxFULL_REPAINT_ON_RESIZE,
                 wxGLCanvasName,
                 wxNullPalette),
      context_(new wxGLContext(this)) {
  }
  void
  on_idle(wxIdleEvent&) {
    this->Refresh(false);
  }
  void
  on_paint(wxPaintEvent&) {
    this->draw();
  }
private:
  void
  draw() {
    // TODO: Use ebiten!
    wxPaintDC dc(this);
    this->SetCurrent(*this->context_);
    this->x_ += 1;
    this->x_ %= 256;
    ::glClearColor(this->x_ / 255.0, 0.5, 1, 0);
    ::glClear(GL_COLOR_BUFFER_BIT);
    ::glFlush();
    this->SwapBuffers();
  }
private:
  wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(gl_canvas, wxGLCanvas)
EVT_IDLE(gl_canvas::on_idle)
EVT_PAINT(gl_canvas::on_paint)
wxEND_EVENT_TABLE()

}
}
}

#endif
