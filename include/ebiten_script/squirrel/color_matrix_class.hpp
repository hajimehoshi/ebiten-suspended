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

#define set_element_iterating(c, i, j) \
  do {                                 \
    ::sq_next(vm, 2);                  \
    SQFloat value;                     \
    ::sq_getfloat(vm, -1, &value);     \
    c->set_element<i, j>(value);       \
    ::sq_pop(vm, 2);                   \
  } while (false)

SQInteger
method_constructor(HSQUIRRELVM vm) {
  try {
    ebiten::graphics::color_matrix* self =
      new ebiten::graphics::color_matrix();
    {
      ::sq_pushnull(vm); // null iterator
      set_element_iterating(self, 0, 0);
      set_element_iterating(self, 0, 1);
      set_element_iterating(self, 0, 2);
      set_element_iterating(self, 0, 3);
      set_element_iterating(self, 0, 4);
      set_element_iterating(self, 1, 0);
      set_element_iterating(self, 1, 1);
      set_element_iterating(self, 1, 2);
      set_element_iterating(self, 1, 3);
      set_element_iterating(self, 1, 4);
      set_element_iterating(self, 2, 0);
      set_element_iterating(self, 2, 1);
      set_element_iterating(self, 2, 2);
      set_element_iterating(self, 2, 3);
      set_element_iterating(self, 2, 4);
      set_element_iterating(self, 3, 0);
      set_element_iterating(self, 3, 1);
      set_element_iterating(self, 3, 2);
      set_element_iterating(self, 3, 3);
      set_element_iterating(self, 3, 4);
      ::sq_pop(vm, 1);
    }
    ::sq_setinstanceup(vm, 1, reinterpret_cast<SQUserPointer>(self));
    ::sq_setreleasehook(vm, 1, releasehook);
    return 0;
  } catch (squirrel_error const& e) {
    return e.sq_value();
  }
}

#undef set_element_iterating

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

#define get_element_guard(v, c, iv, jv, i, j) \
  do {                                        \
    if (iv == i && jv == j) {                 \
      v = c.element<i, j>();                  \
    }                                         \
  } while (false)

SQInteger
metamethod_get(HSQUIRRELVM vm) {
  try {
    ebiten::graphics::color_matrix const& self = get_instance(vm, 1);
    SQChar const* slot_name_p;
    ::sq_getstring(vm, 2, &slot_name_p);
    std::string slot_name(slot_name_p);
    if (slot_name[0] == 'e') {
      // TODO: range check
      std::size_t i = slot_name[1] - '0';
      std::size_t j = slot_name[2] - '0';
      double value = 0;
      // TODO: Refactoring
      get_element_guard(value, self, i, j, 0, 0);
      get_element_guard(value, self, i, j, 0, 1);
      get_element_guard(value, self, i, j, 0, 2);
      get_element_guard(value, self, i, j, 0, 3);
      get_element_guard(value, self, i, j, 0, 4);
      get_element_guard(value, self, i, j, 1, 0);
      get_element_guard(value, self, i, j, 1, 1);
      get_element_guard(value, self, i, j, 1, 2);
      get_element_guard(value, self, i, j, 1, 3);
      get_element_guard(value, self, i, j, 1, 4);
      get_element_guard(value, self, i, j, 2, 0);
      get_element_guard(value, self, i, j, 2, 1);
      get_element_guard(value, self, i, j, 2, 2);
      get_element_guard(value, self, i, j, 2, 3);
      get_element_guard(value, self, i, j, 2, 4);
      get_element_guard(value, self, i, j, 3, 0);
      get_element_guard(value, self, i, j, 3, 1);
      get_element_guard(value, self, i, j, 3, 2);
      get_element_guard(value, self, i, j, 3, 3);
      get_element_guard(value, self, i, j, 3, 4);
      ::sq_pushfloat(vm, value);
    } else {
      std::string msg = "the index '" + slot_name + "' does not exist";
      return ::sq_throwerror(vm, _SC(msg.c_str()));
    }
    return 1;
  } catch (squirrel_error const& e) {
    return e.sq_value();
  }
}

#undef get_element_guard

/*SQInteger
method_values(HSQUIRRELVM vm) {
  try {
    ebiten::graphics::color_matrix const& self = get_instance(vm, 1);
    SQInteger i, j;
    ::sq_getinteger(vm, 2, &i);
    ::sq_getinteger(vm, 3, &j);
    ::sq_pushfloat();
    return 1;
  } catch (squirrel_error const& e) {
    return e.sq_value();
  }
}*/

void
initialize(HSQUIRRELVM vm) {
  HSQOBJECT klass = util::create_class(vm, "ebiten", "ColorMatrix", type_tag());
  util::create_method(vm, klass, "constructor", method_constructor,
                      "xa", false);
  util::create_method(vm, klass, "_get", metamethod_get,
                      "xs", false);
  /*util::create_method(vm, klass, "value", method_value,
    "xii", false);*/
}

}
}

}
}

#endif

