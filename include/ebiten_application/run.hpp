#ifndef EBITEN_APPLICATION_HPP
#define EBITEN_APPLICATION_HPP

#include "ebiten/frames/native_frame.hpp"
#include "ebiten/platform.hpp"

#ifdef EBITEN_MACOSX
# include "ebiten_application/detail/macosx/run.hpp"
#endif

#ifdef EBITEN_IOS
# error "not implemented"
#endif

namespace ebiten_application {
namespace {

void run(ebiten::frames::native_frame native_frame) {
  detail::run(native_frame);
}

}
}

#endif
