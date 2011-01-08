#ifndef EBITEN_UTIL_SINGLETON_HPP
#define EBITEN_UTIL_SINGLETON_HPP

#include <boost/noncopyable.hpp>

namespace ebiten {
namespace util {

template<class T>
class singleton : private boost::noncopyable {
public:
  static T&
  instance() {
    static T instance;
    return instance;
  }
protected:
  singleton() {
  }
  virtual ~singleton() {
  }
};

}
}

#endif
