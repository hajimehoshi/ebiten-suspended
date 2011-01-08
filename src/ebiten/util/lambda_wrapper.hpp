#ifndef EBITEN_UTIL_LAMBDA_WRAPPER_HPP
#define EBITEN_UTIL_LAMBDA_WRAPPER_HPP

/*#include <boost/noncopyable.hpp>
#include <boost/function_types.hpp>

namespace ebiten {
namespace util {

template<int N>
class lambda_wrapper : private boost::noncopyable {
private:
  static std::function<Func> orig_func_;
  static void
  func_() {
    orig_func_();
  }
public:
  lambda_wapper(std::function<void()> func) {
    orig_func_ = func;
  }
  static Func
  func() {
    return func_;
  }
};

static std::function<Func> lambda_wrapper::orig_func_;

}
}*/

#endif
