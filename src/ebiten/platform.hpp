#ifndef EBITEN_PLATFORM_HPP
#define EBITEN_PLATFORM_HPP

#ifdef _WIN32
#define EBITEN_WINDOWS
#endif

#ifdef __APPLE__
#define EBITEN_MACOSX
#endif

#ifdef __linux__
#define EBITEN_LINUX
#endif

#endif
