#ifndef EBITEN_KERNELS_APPLICATION_HPP
#define EBITEN_KERNELS_APPLICATION_HPP

#include "ebiten/platform.hpp"

#ifdef EBITEN_MACOSX
# include "ebiten/kernels/detail/macosx/application.hpp"
#endif

#ifdef EBITEN_IOS
# include "ebiten/kernels/detail/ios/application.hpp"
#endif

namespace ebiten {
namespace kernels {

typedef detail::application application;

}
}


#endif
