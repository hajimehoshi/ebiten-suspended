#ifndef EBITEN_PLATFORM_HPP
#define EBITEN_PLATFORM_HPP

#ifdef wxUSE_GUI
# define EBITEN_WX
#else
# ifdef _WIN32
#  define EBITEN_WINDOWS
# endif
# ifdef __APPLE__
#  include <TargetConditionals.h>
#  if TARGET_OS_IPHONE
#   define EBITEN_IOS
#  else
#   define EBITEN_MACOSX
#  endif
# endif
# ifdef __linux__
#  define EBITEN_LINUX
# endif
#endif

#define EBITEN_OPENGL

#endif
