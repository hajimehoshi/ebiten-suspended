#ifndef SHRIMP_SHRIMP_HPP
#define SHRIMP_SHRIMP_HPP

#include "wx/wx.h"
#include "wx/toolbar.h"

namespace shrimp {

class frame : public wxFrame {
public:
  frame()
    : wxFrame(0,
              -1,
              _T("Shrimp"),
              ::wxPoint(50, 50),
              ::wxSize(450, 340)) {
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

IMPLEMENT_APP(shrimp::app);

#endif
