#ifndef EBITEN_UTIL_ID_HPP
#define EBITEN_UTIL_ID_HPP

#include <cstddef>

namespace ebiten {
namespace util {

class id_ {
public:
  explicit
  id_(void* value)
    : value_(value) {
  }
  explicit
  id_(std::ptrdiff_t value)
    : value_(reinterpret_cast<void*>(value)) {
  }
  operator bool() const {
    return (this->value_ != 0);
  }
  template<class T>
  T
  get() const {
    return static_cast<T>(this->value_);
  }
private:
  void* value_;
};

}
}

#endif
