#ifndef EBITEN_PLATFORM_HPP
#define EBITEN_PLATFORM_HPP

#ifdef _WIN32
#define EBITEN_WINDOWS
#endif

#ifdef __APPLE__
# include <TargetConditionals.h>
# if TARGET_OS_MAC
#  define EBITEN_MACOSX
# elif TARGET_OS_IPHONE
#  define EBITEN_IOS
# endif
#endif

#ifdef __linux__
#define EBITEN_LINUX
#endif

#endif
