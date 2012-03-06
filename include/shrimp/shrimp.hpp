#ifndef SHRIMP_SHRIMP_HPP
#define SHRIMP_SHRIMP_HPP

#define wxNO__T

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
# include <wx/wx.h>
#endif
#include <wx/glcanvas.h>
#include <wx/toolbar.h>
#pragma clang diagnostic pop

#include <iostream>

namespace shrimp {

class gl_canvas : public wxGLCanvas {
public:
  gl_canvas(wxWindow* parent)
    : wxGLCanvas(parent,
                 wxID_ANY,
                 nullptr,
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
    : wxFrame(nullptr,
              wxID_ANY,
              wxT("Shrimp")),
      gl_canvas_(nullptr) {
    {
      wxMenu* file_menu = new wxMenu();
      file_menu->Append(wxID_EXIT, wxT("E&xit\tALT-X"));
      wxMenuBar* menu_bar = new wxMenuBar();
      menu_bar->Append(file_menu, wxT("&File"));
      this->SetMenuBar(menu_bar);
    }
    {
      long style = wxTB_TOP;
      wxToolBar* tool_bar = this->CreateToolBar(style);
      // TODO: Modify tool_bar
      tool_bar->Realize();
    }
    this->gl_canvas_ = new gl_canvas(this);
  }
  virtual void
  OnExit(wxCommandEvent&) {
    this->Close(true);
  }
private:
  DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(frame, wxFrame)
EVT_MENU(wxID_EXIT, frame::OnExit)
END_EVENT_TABLE()

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
