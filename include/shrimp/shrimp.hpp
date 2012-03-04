#ifndef SHRIMP_SHRIMP_HPP
#define SHRIMP_SHRIMP_HPP

// TODO: Check how to use C++11

#pragma clang diagnostic push
// TODO: Ignore -pedantic
#pragma clang diagnostic ignored "-Wall"
#pragma clang diagnostic ignored "-Wextra"
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#pragma clang diagnostic ignored "-Wlong-long"
#pragma clang diagnostic ignored "-Wmissing-prototypes"
#pragma clang diagnostic ignored "-Wshorten-64-to-32"
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/glcanvas.h>
#include <wx/toolbar.h>
#pragma clang diagnostic pop

namespace shrimp {

class gl_canvas : public wxGLCanvas {
public:
  gl_canvas(wxWindow* parent)
    : wxGLCanvas(parent,
                 wxID_ANY,
                 0,
                 wxDefaultPosition,
                 wxDefaultSize,
                 0,
                 wxGLCanvasName,
                 wxNullPalette) {
  }
};

class frame : public wxFrame {
private:
  gl_canvas* gl_canvas_;
public:
  frame()
    : wxFrame(0,
              wxID_ANY,
              _T("Shrimp")),
      gl_canvas_(0) {
    long style = wxTB_TOP;
    wxToolBar* tool_bar = this->CreateToolBar(style);
    // TODO: Modify tool_bar
    tool_bar->Realize();
    this->gl_canvas_ = new gl_canvas(this);
  }
  virtual void
  OnQuit(wxCommandEvent&) {
    this->Close(true);
  }
};

class app : public wxApp {
public:
  virtual bool
  OnInit() {
    frame* f = new frame();
    f->Show(true);
    this->SetTopWindow(f);
    return true;
  }
};

}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-prototypes"
IMPLEMENT_APP(shrimp::app);
#pragma clang diagnostic pop

#endif
