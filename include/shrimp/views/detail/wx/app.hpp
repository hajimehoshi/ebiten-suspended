#ifndef SHRIMP_VIEWS_DETAIL_WX_APP_HPP
#define SHRIMP_VIEWS_DETAIL_WX_APP_HPP

#include "shrimp/views/detail/wx/frame.hpp"
#include "shrimp/views/detail/wx/wx.hpp"

namespace shrimp {
namespace views {
namespace detail {

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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-prototypes"
IMPLEMENT_APP_NO_MAIN(shrimp::views::detail::app);
#pragma clang diagnostic pop

static int
run_main_loop() {
  int argc = 0;
  char** argv = nullptr;
  return wxEntry(argc, argv);
}

}
}
}

#endif
