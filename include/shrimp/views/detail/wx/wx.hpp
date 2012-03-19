#ifndef SHRIMP_VIEWS_DETAIL_WX_WX_HPP
#define SHRIMP_VIEWS_DETAIL_WX_WX_HPP

#define wxNO__T

#ifdef __clang__
# pragma clang diagnostic push
// TODO: Ignore -pedantic
# pragma clang diagnostic ignored "-Wall"
# pragma clang diagnostic ignored "-Wextra"
# pragma clang diagnostic ignored "-Wdeprecated-declarations"
# pragma clang diagnostic ignored "-Wlong-long"
# pragma clang diagnostic ignored "-Wmissing-prototypes"
# pragma clang diagnostic ignored "-Wshorten-64-to-32"
#endif
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
# include <wx/wx.h>
#endif
#include <wx/glcanvas.h>
#include <wx/toolbar.h>
#ifdef __clang__
# pragma clang diagnostic pop
#endif

#endif
