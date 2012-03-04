#ifndef SHRIMP_SHRIMP_HPP
#define SHRIMP_SHRIMP_HPP

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wvariadic-macros"
#pragma clang diagnostic ignored "-Wignored-qualifiers"
#include <wx/wx.h>
#include <wx/toolbar.h>
#pragma clang diagnostic pop

namespace shrimp {

class frame : public wxFrame {
public:
  frame()
    : wxFrame(0,
              wxID_ANY,
              _T("Shrimp"),
              ::wxPoint(50, 50),
              ::wxSize(450, 340)) {
    long style = wxTB_TOP;
    wxToolBar* tool_bar = this->CreateToolBar(style);
    // TODO: Modify tool_bar
    tool_bar->AddSeparator();
    tool_bar->Realize();
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
