#ifndef EBITEN_SCRIPT_SQUIRREL_GEOMETRY_MATRIX_CLASS_HPP
#define EBITEN_SCRIPT_SQUIRREL_GEOMETRY_MATRIX_CLASS_HPP

#include "ebiten_script/squirrel/squirrel_error.hpp"
#include "ebiten_script/squirrel/util.hpp"
#include "ebiten/graphics/geometry_matrix.hpp"
#include <squirrel.h>
#include <cmath>

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
  if (SQ_FAILED(::sq_getinstanceup(vm, idx, &p, type_tag()))) {
    throw squirrel_error(vm);
  }
  ebiten::graphics::geometry_matrix* self =
    reinterpret_cast<ebiten::graphics::geometry_matrix*>(p);
  return *self;
}

SQInteger
metamethod_get(HSQUIRRELVM vm) {
  try {
    ebiten::graphics::geometry_matrix const& self = get_instance(vm, 1);
    SQChar const* slot_name_p;
    ::sq_getstring(vm, 2, &slot_name_p);
    std::string slot_name(slot_name_p);
    if (slot_name == "a") {
      ::sq_pushfloat(vm, self.a());
    } else if (slot_name == "b") {
      ::sq_pushfloat(vm, self.b());
    } else if (slot_name == "c") {
      ::sq_pushfloat(vm, self.c());
    } else if (slot_name == "d") {
      ::sq_pushfloat(vm, self.d());
    } else if (slot_name == "tx") {
      ::sq_pushfloat(vm, self.tx());
    } else if (slot_name == "ty") {
      ::sq_pushfloat(vm, self.ty());
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
method_concat(HSQUIRRELVM vm) {
  try {
    ebiten::graphics::geometry_matrix const& self = get_instance(vm, 1);
    ebiten::graphics::geometry_matrix const& rhs  = get_instance(vm, 2);
    ebiten::graphics::geometry_matrix const result = self.concat(rhs);
    {
      /*
       * [Squirrel]
       * ebiten.GeometryMatrix(result.a, result.b, ...)
       */
      ::sq_getclass(vm, 1);
      ::sq_pushroottable(vm);
      ::sq_pushfloat(vm, result.a());
      ::sq_pushfloat(vm, result.b());
      ::sq_pushfloat(vm, result.c());
      ::sq_pushfloat(vm, result.d());
      ::sq_pushfloat(vm, result.tx());
      ::sq_pushfloat(vm, result.ty());
      if (SQ_FAILED(::sq_call(vm, 7, SQTrue, SQTrue))) {
        throw squirrel_error(vm);
      }
    }
    return 1;
  } catch (squirrel_error const& e) {
    return e.sq_value();
  }
}

SQInteger
static_method_scale(HSQUIRRELVM vm) {
  try {
    HSQOBJECT klass;
    ::sq_getstackobj(vm, 1, &klass);
    SQFloat sx, sy;
    ::sq_getfloat(vm, 2, &sx);
    ::sq_getfloat(vm, 3, &sy);
    {
      /*
       * [Squirrel]
       * identity = ebiten.GeometryMatrix(sx, 0, 0, sy, 0, 0)
       */
      ::sq_pushobject(vm, klass);
      ::sq_pushroottable(vm);
      ::sq_pushfloat(vm, sx);
      ::sq_pushfloat(vm, 0);
      ::sq_pushfloat(vm, 0);
      ::sq_pushfloat(vm, sy);
      ::sq_pushfloat(vm, 0);
      ::sq_pushfloat(vm, 0);
      if (SQ_FAILED(::sq_call(vm, 7, SQTrue, SQTrue))) {
        throw squirrel_error(vm);
      }
    }
    return 1;
  } catch (squirrel_error const& e) {
    return e.sq_value();
  }
}

SQInteger
static_method_rotate(HSQUIRRELVM vm) {
  try {
    HSQOBJECT klass;
    ::sq_getstackobj(vm, 1, &klass);
    SQFloat theta;
    ::sq_getfloat(vm, 2, &theta);
    {
      /*
       * [Squirrel]
       * identity = ebiten.GeometryMatrix(cos(theta), -sin(theta), sin(theta), cos(theta), 0, 0)
       */
      ::sq_pushobject(vm, klass);
      ::sq_pushroottable(vm);
      ::sq_pushfloat(vm, std::cos(theta));
      ::sq_pushfloat(vm, -std::sin(theta));
      ::sq_pushfloat(vm, std::sin(theta));
      ::sq_pushfloat(vm, std::cos(theta));
      ::sq_pushfloat(vm, 0);
      ::sq_pushfloat(vm, 0);
      if (SQ_FAILED(::sq_call(vm, 7, SQTrue, SQTrue))) {
        throw squirrel_error(vm);
      }
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
  util::create_method(vm, klass, "concat", method_concat,
                      "xx", false);
  util::create_method(vm, klass, "scale", static_method_scale,
                      "ynn", true);
  util::create_method(vm, klass, "rotate", static_method_rotate,
                      "yn", true);
  HSQOBJECT identity;
  {
    /*
     * [Squirrel]
     * identity = ebiten.GeometryMatrix(1, 0, 0, 1, 0, 0)
     */
    SQInteger const top = ::sq_gettop(vm);
    ::sq_pushobject(vm, klass);
    ::sq_pushroottable(vm);
    ::sq_pushfloat(vm, 1);
    ::sq_pushfloat(vm, 0);
    ::sq_pushfloat(vm, 0);
    ::sq_pushfloat(vm, 1);
    ::sq_pushfloat(vm, 0);
    ::sq_pushfloat(vm, 0);
    ::sq_call(vm, 7, SQTrue, SQTrue);
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
