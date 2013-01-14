#ifndef EBITEN_VERSION_HPP
#define EBITEN_VERSION_HPP

#include <ctime>
#include <string>

//preprocessor magic for the commit hash macro

#define EBITEN_QUOTE(str) #str
#define EBITEN_EXPAND_AND_QUOTE(str) EBITEN_QUOTE(str)

#ifndef EBITEN_VERSION_COMMIT_UNIX_TIME
# warning "EBITEN_VERSION_COMMIT_UNIX_TIME is not defined!"
# define EBITEN_VERSION_COMMIT_UNIX_TIME (0)
#endif

//on Xcode, the commit hash (and others) are defined in an .xcconfig
//generated at build time.
#ifdef EBITEN_VERSION_COMMIT_HASH_XCODE
#define EBITEN_VERSION_COMMIT_HASH EBITEN_EXPAND_AND_QUOTE(EBITEN_VERSION_COMMIT_HASH_XCODE)
#endif

#ifndef EBITEN_VERSION_COMMIT_HASH
# warning "EBITEN_VERSION_COMMIT_HASH is not defined!"
# define EBITEN_VERSION_COMMIT_HASH "(unknown)"
#endif

#ifndef EBITEN_VERSION_COMMIT_MODIFIED
# define EBITEN_VERSION_COMMIT_MODIFIED (0)
#endif

namespace ebiten {
namespace version {
namespace {

// TODO: Improve that

std::string
get_commit_time() {
  time_t const time = EBITEN_VERSION_COMMIT_UNIX_TIME;
  tm const* time_tm = ::gmtime(&time);
  char output[] = "YYYY-MM-DDTHH:MMZ";
  ::strftime(output, sizeof(output), "%FT%RZ", time_tm);
  return std::string(output, sizeof(output) - 1);
}

std::string
get_commit_hash() {
  return EBITEN_VERSION_COMMIT_HASH;
}

bool
is_commit_modified() {
  return static_cast<bool>(EBITEN_VERSION_COMMIT_MODIFIED);
}

}
}
}

#define EBITEN_VERSION          (00000)
#define EBITEN_VERSION_MAJOR    (EBITEN_VERSION / 10000)
#define EBITEN_VERSION_MINOR    ((EBITEN_VERSION / 100) % 100)
#define EBITEN_VERSION_SUBMINOR (EBITEN_VERSION % 100)

#endif
