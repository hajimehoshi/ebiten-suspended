#ifndef SHRIMP_VIEWS_DETAIL_WX_FRAME_HPP
#define SHRIMP_VIEWS_DETAIL_WX_FRAME_HPP

#include "shrimp/views/detail/wx/gl_canvas.hpp"
#include "shrimp/views/detail/wx/wx.hpp"
#include "ebiten/ebiten.hpp"
#include <functional>
#include <memory>

namespace shrimp {
namespace views {
namespace detail {

class frame : public wxFrame {
private:
  class map_editor_drawer {
  public:
    bool
    update(ebiten::graphics::texture_factory&,
           ebiten::input const&) {
      return false;
    }
    void
    draw(ebiten::graphics::graphics_context& g,
         ebiten::graphics::texture& main_offscreen) {
      g.set_offscreen(main_offscreen);
      g.clear();
      g.draw_rect(50, 50, 100, 100,
                  0, 128, 255, 255);
    }
  };
  std::unique_ptr<map_editor_drawer> map_editor_drawer_;
private:
  gl_canvas* map_editor_;
public:
  frame()
    : wxFrame(nullptr,
              wxID_ANY,
              wxT("Shrimp")),
      map_editor_(nullptr) {
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
    auto update = std::bind(&map_editor_drawer::update,
                            this->map_editor_drawer_.get(),
                            std::placeholders::_1,
                            std::placeholders::_2);
    auto draw = std::bind(&map_editor_drawer::draw,
                          this->map_editor_drawer_.get(),
                          std::placeholders::_1,
                          std::placeholders::_2);
    this->map_editor_ = new gl_canvas(this, update, draw);
  }
  void
  on_exit(wxCommandEvent&) {
    this->Close(true);
  }
private:
  wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(frame, wxFrame)
EVT_MENU(wxID_EXIT, frame::on_exit)
wxEND_EVENT_TABLE()

}
}
}

#endif
