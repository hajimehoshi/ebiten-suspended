#ifndef SHRIMP_VIEWS_DETAIL_WX_FRAME_HPP
#define SHRIMP_VIEWS_DETAIL_WX_FRAME_HPP

#include "shrimp/views/detail/wx/gl_canvas.hpp"
#include "shrimp/views/detail/wx/wx.hpp"

namespace shrimp {
namespace views {
namespace detail {

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

}
}
}

#endif
