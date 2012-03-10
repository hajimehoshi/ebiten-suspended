#ifndef SHRIMP_VIEWS_DETAIL_WX_GL_CANVS_HPP
#define SHRIMP_VIEWS_DETAIL_WX_GL_CANVS_HPP

#include "shrimp/views/detail/wx/wx.hpp"

namespace shrimp {
namespace views {
namespace detail {

class gl_canvas : public wxGLCanvas {
private:
  wxGLContext* context_;
public:
  gl_canvas(wxWindow* parent)
    : wxGLCanvas(parent,
                 wxID_ANY,
                 nullptr,
                 wxDefaultPosition,
                 wxDefaultSize,
                 0,
                 wxGLCanvasName,
                 wxNullPalette),
      context_(new wxGLContext(this)) {
  }
  void
  OnPaint(wxPaintEvent&) {
    // TODO: Make design?
    // TODO: Use ebiten!
    wxPaintDC dc(this);
    this->SetCurrent(*this->context_);
    ::glClearColor(0, 0, 0, 0);
    ::glClear(GL_COLOR_BUFFER_BIT);
    ::glFlush();
    this->SwapBuffers();
  }
private:
  DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(gl_canvas, wxGLCanvas)
EVT_PAINT(gl_canvas::OnPaint)
END_EVENT_TABLE()

}
}
}

#endif
