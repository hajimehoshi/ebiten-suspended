#ifndef SHRIMP_VIEWS_DETAIL_WX_GL_CANVS_HPP
#define SHRIMP_VIEWS_DETAIL_WX_GL_CANVS_HPP

#include "shrimp/views/detail/wx/wx.hpp"

namespace shrimp {
namespace views {
namespace detail {

class gl_canvas : public wxGLCanvas {
private:
  class refresh_timer : public wxTimer {
  private:
    wxWindow* window_;
  public:
    refresh_timer(wxWindow* parent)
      : wxTimer(parent),
        window_(parent) {
    }
    void
    Notify() {
      this->window_->Refresh(false);
    }
  };
private:
  wxGLContext* context_;
  refresh_timer* timer_;
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
      context_(new wxGLContext(this)),
      timer_(new refresh_timer(this)) {
    this->timer_->Start(16);
  }
  void
  on_close(wxCloseEvent&) {
    this->timer_->Stop();
  }
  void
  on_paint(wxPaintEvent&) {
    this->draw();
  }
  void
  on_timer(wxTimerEvent&) {
    this->Refresh(false);
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
EVT_CLOSE(gl_canvas::on_close)
EVT_PAINT(gl_canvas::on_paint)
wxEND_EVENT_TABLE()

}
}
}

#endif
