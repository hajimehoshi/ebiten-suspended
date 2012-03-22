#ifndef SHRIMP_VIEWS_DETAIL_WX_MAIN_FRAME_HPP
#define SHRIMP_VIEWS_DETAIL_WX_MAIN_FRAME_HPP

#include "shrimp/views/detail/wx/gl_canvas.hpp"
#include "shrimp/views/detail/wx/wx.hpp"
#include "ebiten/ebiten.hpp"
#include <wx/splitter.h>
#include <functional>

namespace shrimp {
namespace views {
namespace detail {

class main_frame : public wxFrame {
private:
  std::function<void(ebiten::graphics::graphics_context&,
                     ebiten::graphics::texture&)> draw_map_editor_func_;
public:
  main_frame()
    : wxFrame(nullptr,
              wxID_ANY,
              wxT("Shrimp"),
              wxDefaultPosition,
              wxSize(800, 600)) {
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
    auto main_splitter = new wxSplitterWindow(this,
                                              wxID_ANY,
                                              wxDefaultPosition,
                                              wxDefaultSize,
                                              wxSP_LIVE_UPDATE | wxSP_NOBORDER);
    {
      auto source_list_panel = new wxPanel(main_splitter);
      auto model_panel       = new wxPanel(main_splitter);
      //left_panel->SetBackgroundColour(wxColour(0xc6, 0xd0, 0xda));
      main_splitter->SplitVertically(source_list_panel,
                                     model_panel,
                                     120);
      // model
      {
        auto model_splitter = new wxSplitterWindow(model_panel,
                                                   wxID_ANY,
                                                   wxDefaultPosition,
                                                   wxDefaultSize,
                                                   wxSP_LIVE_UPDATE | wxSP_NOBORDER);
        {
          auto sizer = new wxBoxSizer(wxHORIZONTAL);
          sizer->Add(model_splitter, 1, wxEXPAND, 0);
          model_panel->SetSizer(sizer);
        }
        auto entries_panel = new wxPanel(model_splitter);
        auto editor_panel  = new wxPanel(model_splitter);
        model_splitter->SplitVertically(entries_panel,
                                        editor_panel,
                                        120);
        // map editor
        {
          auto update = std::bind(&main_frame::on_update_map_editor,
                                  this,
                                  std::placeholders::_1,
                                  std::placeholders::_2);
          auto draw = std::bind(&main_frame::on_draw_map_editor,
                                this,
                                std::placeholders::_1,
                                std::placeholders::_2);
          auto c = new gl_canvas(editor_panel,
                                 update, draw);
          auto sizer = new wxBoxSizer(wxHORIZONTAL);
          sizer->Add(c, 1, wxEXPAND, 0);
          editor_panel->SetSizer(sizer);
        }
      }
    }
  }
  void
  set_draw_map_editor_func(std::function<void(ebiten::graphics::graphics_context&,
                                              ebiten::graphics::texture&)> const &func) {
    this->draw_map_editor_func_ = func;
  }
  void
  on_exit(wxCommandEvent&) {
    this->draw_map_editor_func_ = nullptr;
    this->Close(true);
  }
  void
  set_source_map() {
  }
private:
  bool
  on_update_map_editor(ebiten::graphics::texture_factory&,
                       ebiten::input const&) {
    return false;
  }
  void
  on_draw_map_editor(ebiten::graphics::graphics_context& g,
                     ebiten::graphics::texture& offscreen) {
    if (!this->draw_map_editor_func_) {
      return;
    }
    this->draw_map_editor_func_(g, offscreen);
  }
  wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(main_frame, wxFrame)
EVT_MENU(wxID_EXIT, main_frame::on_exit)
wxEND_EVENT_TABLE()

}
}
}

#endif