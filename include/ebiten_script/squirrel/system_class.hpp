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
method_constructor(HSQUIRRELVM vm) {
  try {
    ::sq_pushstring(vm, _SC("input_"), -1);
    ::sq_push(vm, 2);
    if (SQ_FAILED(::sq_set(vm, 1))) {
      throw squirrel_error(vm);
    }
    return 0;
  } catch (squirrel_error const& e) {
    return e.sq_value();
  }
}

SQInteger
metamethod_get(HSQUIRRELVM vm) {
  try {
    SQChar const* slot_name_p;
    ::sq_getstring(vm, 2, &slot_name_p);
    std::string slot_name(slot_name_p);
    if (slot_name == "input") {
      ::sq_pushstring(vm, _SC("input_"), -1);
      if (SQ_FAILED(::sq_get(vm, 1))) {
        throw squirrel_error(vm);
      }
    } else {
      std::string msg = "the index '" + slot_name + "' does not exist";
      return ::sq_throwerror(vm, _SC(msg.c_str()));
    }
    return 1;
  } catch (squirrel_error const& e) {
    return e.sq_value();
  }
}

void
initialize(HSQUIRRELVM vm) {
  HSQOBJECT klass = util::create_class(vm, "ebiten", "System", type_tag());
  util::create_method(vm, klass, "constructor", method_constructor,
                      "xx", false);
  util::create_method(vm, klass, "_get", metamethod_get,
                      "xs", false);
  util::create_null_variable(vm, klass, "input_");
}

}
}

}
}

#endif
