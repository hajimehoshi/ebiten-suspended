#ifndef EBITEN_NONCOPYABLE_HPP
#define EBITEN_NONCOPYABLE_HPP

namespace ebiten {

namespace avoid_adl {

class noncopyable {
public:
  noncopyable() = default;
  noncopyable(noncopyable const&) = delete;
  noncopyable& operator=(noncopyable const&) = delete;
  noncopyable(noncopyable&&) = default;
  noncopyable& operator=(noncopyable&&) = default;
};

}

typedef avoid_adl::noncopyable noncopyable;

}

#endif
