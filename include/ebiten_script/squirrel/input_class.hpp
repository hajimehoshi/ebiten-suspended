#ifndef EBITEN_SCRIPT_SQUIRREL_INPUT_CLASS_HPP
#define EBITEN_SCRIPT_SQUIRREL_INPUT_CLASS_HPP

#include "ebiten_script/squirrel/squirrel_error.hpp"
#include "ebiten_script/squirrel/util.hpp"
#include "ebiten/input.hpp"
#include <squirrel.h>
#include <tuple>

namespace ebiten_script {
namespace squirrel {

namespace input_class {
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

ebiten::input const&
get_instance(HSQUIRRELVM vm, SQInteger idx) {
  SQUserPointer p;
  if (SQ_FAILED(::sq_getinstanceup(vm, idx, &p, type_tag()))) {
    throw squirrel_error(vm);
  }
  ebiten::input const* self = reinterpret_cast<ebiten::input*>(p);
  if (!self) {
    throw squirrel_error(vm, "input is not set");
  }
  return *self;
}

SQInteger
metamethod_get(HSQUIRRELVM vm) {
  try {
    ebiten::input const& self = get_instance(vm, 1);
    SQChar const* slot_name_p;
    ::sq_getstring(vm, 2, &slot_name_p);
    std::string slot_name(slot_name_p);
    if (slot_name == "touchX") {
      int touch_x = std::get<0>(self.touches(0));
      ::sq_pushinteger(vm, touch_x);
    } else if (slot_name == "touchY") {
      int touch_y = std::get<1>(self.touches(0));
      ::sq_pushinteger(vm, touch_y);
    } else if (slot_name == "isTouched") {
      bool is_touched = self.is_touched(0);
      ::sq_pushbool(vm, is_touched);
    } else {
      std::string msg = "the index '" + slot_name + "' does not exist";
      return ::sq_throwerror(vm, _SC(msg.c_str()));
    }
    return 1;
  } catch (squirrel_error const& e) {
    return e.sq_value();
  }
}

SQInteger
method_set_input(HSQUIRRELVM vm) {
  try {
    SQUserPointer p;
    ::sq_getuserpointer(vm, 2, &p);
    ::sq_setinstanceup(vm, 1, p);
    return 0;
  } catch (squirrel_error const& e) {
    return e.sq_value();
  }
}

void
initialize(HSQUIRRELVM vm) {
  HSQOBJECT klass = util::create_class(vm, "ebiten", "Input", type_tag());
  util::create_method(vm, klass, "constructor", method_constructor,
                      "x", false);
  util::create_method(vm, klass, "_get", metamethod_get,
                      "xs", false);
  util::create_method(vm, klass, "setInput_", method_set_input,
                      "xp", false);
}

}
}

}
}

#endif
