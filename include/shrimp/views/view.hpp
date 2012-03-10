#ifndef SHRIMP_VIEWS_VIEW_HPP
#define SHRIMP_VIEWS_VIEW_HPP

#include "shrimp/views/detail/wx/app.hpp"

namespace shrimp {
namespace views {

}
}

// TODO: Use IMPLEMENT_APP_NO_MAIN
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-prototypes"
IMPLEMENT_APP(shrimp::views::detail::app);
#pragma clang diagnostic pop

#endif
