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

}
}
}

#endif
