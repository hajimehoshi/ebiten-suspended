#ifndef SHRIMP_VIEWS_VIEW_HPP
#define SHRIMP_VIEWS_VIEW_HPP

#include "shrimp/views/detail/wx/app.hpp"

namespace shrimp {
namespace views {

static int
run_main_loop() {
  return detail::run_main_loop();
}

}
}

#endif
