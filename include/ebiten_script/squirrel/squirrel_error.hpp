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
  squirrel_error(SQInteger sq_value) 
    : std::runtime_error("Squirrel error"),
      sq_value_(sq_value) {
  }
  SQInteger
  sq_value() const {
    return this->sq_value_;
  }
};

}
}

#endif

