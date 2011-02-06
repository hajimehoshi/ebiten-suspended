#ifndef EBITEN_UTIL_ID_HPP
#define EBITEN_UTIL_ID_HPP

#include <boost/noncopyable.hpp>
#include <cstddef>

namespace ebiten {
namespace util {

class id_ : boost::noncopyable {
public:
  id_(void* value)
    : value_(value) {
  }
  template<class T>
  T
  get() const {
    return reinterpret_cast<T>(this->value_);
  }
private:
  void* value_;
};

}
}

#endif
