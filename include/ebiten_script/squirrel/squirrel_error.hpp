#ifndef EBITEN_SCRIPT_SQUIRREL_SQUIRREL_ERROR_HPP
#define EBITEN_SCRIPT_SQUIRREL_SQUIRREL_ERROR_HPP

#include <squirrel.h>
#include <stdexcept>
#include <string>

namespace ebiten_script {
namespace squirrel {

class squirrel_error : public std::runtime_error {
private:
  SQInteger sq_value_;
public:
  explicit
  squirrel_error(HSQUIRRELVM vm)
    : std::runtime_error(get_message(vm)) {
    ::sq_getlasterror(vm);
    this->sq_value_ = ::sq_throwobject(vm);
  }
  squirrel_error(HSQUIRRELVM vm, std::string const& message) 
    : std::runtime_error(message),
      sq_value_(::sq_throwerror(vm, _SC(message.c_str()))) {
  }
  SQInteger
  sq_value() const {
    return this->sq_value_;
  }
private:
  std::string
  get_message(HSQUIRRELVM vm) {
    SQInteger const top = ::sq_gettop(vm);
    ::sq_getlasterror(vm);
    SQChar const* message_p;
    ::sq_getstring(vm, -1, &message_p);
    std::string message(message_p);
    ::sq_settop(vm, top);
    return message;
  }
};

}
}

#endif

