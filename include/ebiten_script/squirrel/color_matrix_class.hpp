#ifndef EBITEN_SCRIPT_SQUIRREL_COLOR_MATRIX_CLASS_HPP
#define EBITEN_SCRIPT_SQUIRREL_COLOR_MATRIX_CLASS_HPP

#include "ebiten_script/squirrel/squirrel_error.hpp"
#include "ebiten_script/squirrel/util.hpp"
#include "ebiten/graphics/color_matrix.hpp"
#include <squirrel.h>
#include <array>

namespace ebiten_script {
namespace squirrel {

namespace color_matrix_class {
namespace {

SQInteger
method_constructor(HSQUIRRELVM vm);
SQInteger
releasehook(SQUserPointer p, SQInteger);

SQUserPointer
type_tag() {
  return reinterpret_cast<SQUserPointer>(method_constructor);
}

SQInteger
method_constructor(HSQUIRRELVM vm) {
  try {
    std::array<SQFloat, 20> elements;
    {
      util::stack_restorer r(vm);
      ::sq_pushnull(vm);
      for (auto it = elements.begin(); it != elements.end(); ++it) {
        ::sq_next(vm, 2);
        SQFloat value;
        ::sq_getfloat(vm, -1, &value);
        ::sq_pop(vm, 2);
        *it = value;
      }
    }
    ebiten::graphics::color_matrix* self =
      new ebiten::graphics::color_matrix(elements.begin(), elements.end());
    ::sq_setinstanceup(vm, 1, reinterpret_cast<SQUserPointer>(self));
    ::sq_setreleasehook(vm, 1, releasehook);
    return 0;
  } catch (squirrel_error const& e) {
    return e.sq_value();
  }
}

SQInteger
releasehook(SQUserPointer p, SQInteger) {
  ebiten::graphics::color_matrix* self =
    reinterpret_cast<ebiten::graphics::color_matrix*>(p);
  delete self;
  return 1;
}

ebiten::graphics::color_matrix const&
get_instance(HSQUIRRELVM vm, SQInteger idx) {
  SQUserPointer p;
  if (SQ_FAILED(::sq_getinstanceup(vm, idx, &p, type_tag()))) {
    throw squirrel_error(vm);
  }
  ebiten::graphics::color_matrix* self =
    reinterpret_cast<ebiten::graphics::color_matrix*>(p);
  return *self;
}

SQInteger
metamethod_get(HSQUIRRELVM vm) {
  try {
    ebiten::graphics::color_matrix const& self = get_instance(vm, 1);
    SQChar const* slot_name_p;
    ::sq_getstring(vm, 2, &slot_name_p);
    std::string slot_name(slot_name_p);
    if (slot_name[0] == 'e') {
      // TODO: length check
      // TODO: range check
      std::size_t i = slot_name[1] - '0';
      std::size_t j = slot_name[2] - '0';
      ::sq_pushfloat(vm, self.element(i, j));
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
  HSQOBJECT klass = util::create_class(vm, "ebiten", "ColorMatrix", type_tag());
  util::create_method(vm, klass, "constructor", method_constructor,
                      "xa", false);
  util::create_method(vm, klass, "_get", metamethod_get,
                      "xs", false);
  HSQOBJECT ebiten;
  {
    util::stack_restorer r(vm);
    ::sq_pushroottable(vm);
    ::sq_pushstring(vm, _SC("ebiten"), -1);
    ::sq_rawget(vm, -2);
    ::sq_getstackobj(vm, -1, &ebiten);
  }
  {
    util::stack_restorer r(vm);
    ::sq_newarray(vm, 20);
    HSQOBJECT arr;
    ::sq_getstackobj(vm, -1, &arr);
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 5; j++) {
        util::stack_restorer r(vm);
        ::sq_pushinteger(vm, i * 5 + j);
        if (i == j) {
          ::sq_pushinteger(vm, 1);
        } else {
          ::sq_pushinteger(vm, 0);
        }
        ::sq_rawset(vm, -3);
      }
    }
    HSQOBJECT identity = util::call(vm, ebiten, "ColorMatrix", true, arr);
    util::create_variable(vm, klass, "identity", identity, true);
  }
}

}
}

}
}

#endif

