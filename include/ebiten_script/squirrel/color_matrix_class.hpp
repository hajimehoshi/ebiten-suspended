#ifndef EBITEN_SCRIPT_SQUIRREL_COLOR_MATRIX_CLASS_HPP
#define EBITEN_SCRIPT_SQUIRREL_COLOR_MATRIX_CLASS_HPP

#include "ebiten_script/squirrel/squirrel_error.hpp"
#include "ebiten_script/squirrel/util.hpp"
#include "ebiten/graphics/color_matrix.hpp"
#include <squirrel.h>

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
    ebiten::graphics::color_matrix* self =
      new ebiten::graphics::color_matrix();
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
method_values(HSQUIRRELVM vm) {
  try {
    ebiten::graphics::color_matrix const& self = get_instance(vm, 1);
    /*SQInteger i, j;
    ::sq_getinteger(vm, 2, &i);
    ::sq_getinteger(vm, 3, &j);
    ::sq_pushfloat();*/
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
  /*util::create_method(vm, klass, "value", method_value,
    "xii", false);*/
}

}
}

}
}

#endif

