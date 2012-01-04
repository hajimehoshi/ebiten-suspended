#ifndef EBITEN_VERSION_HPP
#define EBITEN_VERSION_HPP

#include <ctime>
#include <string>

#ifndef EBITEN_VERSION_COMMIT_UNIX_TIME
# warning "EBITEN_VERSION_COMMIT_DATE is not defined!"
# define EBITEN_VERSION_COMMIT_UNIX_TIME (0)
#endif

#ifndef EBITEN_VERSION_COMMIT_HASH
# warning "EBITEN_VERSION_COMMIT_HASH is not defined!"
# define EBITEN_VERSION_COMMIT_HASH "(unknown)"
#endif

namespace ebiten {
namespace version {
namespace {

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

}
}
}

#define EBITEN_VERSION          (00000)
#define EBITEN_VERSION_MAJOR    (EBITEN_VERSION / 10000)
#define EBITEN_VERSION_MINOR    ((EBITEN_VERSION / 100) % 100)
#define EBITEN_VERSION_SUBMINOR (EBITEN_VERSION % 100)

#endif
