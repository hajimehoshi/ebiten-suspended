#ifndef SHRIMP_VIEWS_DETAIL_WX_APP_HPP
#define SHRIMP_VIEWS_DETAIL_WX_APP_HPP

#include "shrimp/views/detail/wx/main_frame.hpp"
#include "shrimp/views/detail/wx/wx.hpp"
#include "ebiten/noncopyable.hpp"

namespace shrimp {
namespace views {
namespace detail {

class app : private ebiten::noncopyable {
private:
  main_frame* main_frame_;
public:
  app()
    : main_frame_(nullptr) {
    wxApp::SetInstance(new wxApp());
    int argc = 0;
    char** argv = nullptr;
    ::wxEntryStart(argc, argv);
    wxTheApp->OnInit();
    {
      wxTheApp->SetExitOnFrameDelete(true);
      this->main_frame_ = new class main_frame();
      this->main_frame_->Show(true);
      wxTheApp->SetTopWindow(this->main_frame_);
    }
  }
  ~app() {
    wxTheApp->OnExit();
    ::wxEntryCleanup();
  }
  int
  run() {
    return wxTheApp->OnRun();
  }
  class main_frame&
  main_frame() {
    return *this->main_frame_;
  }
  class main_frame const&
  main_frame() const {
    return *this->main_frame_;
  }
};

}
}
}

#endif
