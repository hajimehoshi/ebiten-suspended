#ifndef EBITEN_UTIL_NONCOPYABLE_HPP
#define EBITEN_UTIL_NONCOPYABLE_HPP

namespace ebiten {
namespace util {

namespace avoid_adl {

class noncopyable {
protected:
  noncopyable() {}
  ~noncopyable() {}
private:
  noncopyable(noncopyable const&);
  noncopyable const& operator=(noncopyable const&);
};

}

typedef avoid_adl::noncopyable noncopyable;

}
}

#endif
