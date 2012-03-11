#ifndef SHRIMP_VIEWS_DETAIL_WX_APP_HPP
#define SHRIMP_VIEWS_DETAIL_WX_APP_HPP

#include "shrimp/views/detail/wx/frame.hpp"
#include "shrimp/views/detail/wx/view.hpp"
#include "shrimp/views/detail/wx/wx.hpp"
#include "ebiten/noncopyable.hpp"

namespace shrimp {
namespace views {
namespace detail {

class app : private ebiten::noncopyable {
private:
  view view_;
public:
  app()
    : view_() {
    wxApp::SetInstance(new wxApp());
    int argc = 0;
    char** argv = nullptr;
    ::wxEntryStart(argc, argv);
    wxTheApp->OnInit();
    {
      wxTheApp->SetExitOnFrameDelete(true);
      frame* frame_ = new frame();
      frame_->Show(true);
      wxTheApp->SetTopWindow(frame_);
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
};

}
}
}

#endif
