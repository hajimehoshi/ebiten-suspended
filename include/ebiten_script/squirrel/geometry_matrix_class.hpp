#ifndef EBITEN_SCRIPT_SQUIRREL_GEOMETRY_MATRIX_CLASS_HPP
#define EBITEN_SCRIPT_SQUIRREL_GEOMETRY_MATRIX_CLASS_HPP

#include "ebiten_script/squirrel/squirrel_error.hpp"
#include "ebiten/graphics/geometry_matrix.hpp"
#include <squirrel.h> 

namespace ebiten_script {
namespace squirrel {

namespace geometry_matrix_class {
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
    SQFloat a, b, c, d, tx, ty;
    ::sq_getfloat(vm, 2, &a);
    ::sq_getfloat(vm, 3, &b);
    ::sq_getfloat(vm, 4, &c);
    ::sq_getfloat(vm, 5, &d);
    ::sq_getfloat(vm, 6, &tx);
    ::sq_getfloat(vm, 7, &ty);
    ebiten::graphics::geometry_matrix* self =
      new ebiten::graphics::geometry_matrix(a, b, c, d, tx, ty);
    ::sq_setinstanceup(vm, 1, reinterpret_cast<SQUserPointer>(self));
    ::sq_setreleasehook(vm, 1, releasehook);
    return 0;
  } catch (squirrel_error const& e) {
    return e.sq_value();
  }
}

SQInteger
releasehook(SQUserPointer p, SQInteger) {
  class sprite* self = reinterpret_cast<sprite*>(p);
  delete self;
  return 1;
}

ebiten::graphics::geometry_matrix&
get_instance(HSQUIRRELVM vm, SQInteger idx) {
  SQUserPointer p;
  {
    SQRESULT result = ::sq_getinstanceup(vm, idx, &p, type_tag());
    if (SQ_FAILED(result)) {
      throw squirrel_error(result);
    }
  }
  ebiten::graphics::geometry_matrix* self =
    reinterpret_cast<ebiten::graphics::geometry_matrix*>(p);
  return *self;
}

SQInteger
meta_method_get(HSQUIRRELVM vm) {
  try {
    ebiten::graphics::geometry_matrix& self = get_instance(vm, 1);
    SQChar const* method_name_p;
    ::sq_getstring(vm, 2, &method_name_p);
    std::string method_name(method_name_p);
    if (method_name == "a") {
      ::sq_pushfloat(vm, self.a());
    } else if (method_name == "b") {
      ::sq_pushfloat(vm, self.b());
    } else if (method_name == "c") {
      ::sq_pushfloat(vm, self.c());
    } else if (method_name == "d") {
      ::sq_pushfloat(vm, self.d());
    } else if (method_name == "tx") {
      ::sq_pushfloat(vm, self.tx());
    } else if (method_name == "ty") {
      ::sq_pushfloat(vm, self.ty());
    } else {
      std::string msg = "the index '" + method_name + "' does not exist";
      return ::sq_throwerror(vm, _SC(msg.c_str()));
    }
    return 1;
  } catch (squirrel_error const& e) {
    return e.sq_value();
  }
}

}
}

}
}

#endif
