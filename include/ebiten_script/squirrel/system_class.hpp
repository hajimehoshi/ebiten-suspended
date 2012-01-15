#ifndef EBITEN_SCRIPT_SQUIRREL_SYSTEM_CLASS_HPP
#define EBITEN_SCRIPT_SQUIRREL_SYSTEM_CLASS_HPP

#include "ebiten_script/squirrel/squirrel_error.hpp"
#include "ebiten_script/squirrel/util.hpp"
#include <squirrel.h>

namespace ebiten_script {
namespace squirrel {

namespace system_class {
namespace {

SQInteger
method_constructor(HSQUIRRELVM vm);

SQUserPointer
type_tag() {
  return reinterpret_cast<SQUserPointer>(method_constructor);
}

SQInteger
method_constructor(HSQUIRRELVM) {
  try {
    return 0;
  } catch (squirrel_error const& e) {
    return e.sq_value();
  }
}

void
initialize(HSQUIRRELVM vm) {
  HSQOBJECT klass = util::create_class(vm, "ebiten", "System", type_tag());
  util::create_method(vm, klass, "constructor", method_constructor,
                      "x", false);
}

}
}

}
}

#endif
