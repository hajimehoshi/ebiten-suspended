#ifndef EBITEN_SCRIPT_SQUIRREL_SQUIRREL_ERROR_HPP
#define EBITEN_SCRIPT_SQUIRREL_SQUIRREL_ERROR_HPP

#include <squirrel.h>
#include <exception>

namespace ebiten_script {
namespace squirrel {

class squirrel_error : public std::runtime_error {
private:
  SQInteger sq_value_;
public:
  squirrel_error(HSQUIRRELVM vm)
    : std::runtime_error("Squirrel error") {
    ::sq_getlasterror(vm);
    this->sq_value_ = ::sq_throwobject(vm);
  }
  squirrel_error(HSQUIRRELVM vm, std::string const& message) 
    : std::runtime_error("Squirrel error"),
      sq_value_(::sq_throwerror(vm, _SC(message.c_str()))) {
  }
  SQInteger
  sq_value() const {
    return this->sq_value_;
  }
};

}
}

#endif

