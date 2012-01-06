#ifndef EBITEN_SCRIPT_SQUIRREL_GEOMETRY_MATRIX_CLASS_HPP
#define EBITEN_SCRIPT_SQUIRREL_GEOMETRY_MATRIX_CLASS_HPP

#include "ebiten_script/squirrel/squirrel_error.hpp"
#include "ebiten_script/squirrel/util.hpp"
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
  ebiten::graphics::geometry_matrix* self =
    reinterpret_cast<ebiten::graphics::geometry_matrix*>(p);
  delete self;
  return 1;
}

ebiten::graphics::geometry_matrix const&
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
metamethod_get(HSQUIRRELVM vm) {
  try {
    ebiten::graphics::geometry_matrix const& self = get_instance(vm, 1);
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

void
initialize(HSQUIRRELVM vm) {
  HSQOBJECT klass = util::create_class(vm, "ebiten", "GeometryMatrix", type_tag());
  util::create_method(vm, klass, "constructor", method_constructor,
                      "xnnnnnn", false);
  util::create_method(vm, klass, "_get", metamethod_get,
                      "xs", false);
  HSQOBJECT identity;
  {
    /*
     * [Squirrel]
     * identity = ebiten.GeometryMatrix(1, 0, 0, 1, 0, 0)
     */
    SQInteger const top = ::sq_gettop(vm);
    ::sq_pushobject(vm, klass);
    ::sq_pushroottable(vm);
    ::sq_pushinteger(vm, 1);
    ::sq_pushinteger(vm, 0);
    ::sq_pushinteger(vm, 0);
    ::sq_pushinteger(vm, 1);
    ::sq_pushinteger(vm, 0);
    ::sq_pushinteger(vm, 0);
    if (SQ_FAILED(::sq_call(vm, 7, SQTrue, SQTrue))) {
      ::sq_getlasterror(vm);
      SQChar const* str;
      ::sq_getstring(vm, -1, &str);
      throw std::runtime_error(str);
    }
    ::sq_getstackobj(vm, -1, &identity);
    ::sq_addref(vm, &identity);
    ::sq_settop(vm, top);
  }
  util::create_variable(vm, klass, "identity", identity, true);
}

}
}

}
}

#endif
