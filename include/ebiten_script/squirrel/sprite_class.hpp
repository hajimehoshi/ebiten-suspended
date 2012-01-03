#ifndef EBITEN_SCRIPT_SQUIRREL_SPRITE_CLASS_HPP
#define EBITEN_SCRIPT_SQUIRREL_SPRITE_CLASS_HPP

#include "ebiten_script/squirrel/squirrel_error.hpp"
#include "ebiten_script/squirrel/texture_class.hpp"
#include "ebiten_script/sprite.hpp"
#include <squirrel.h>

namespace ebiten_script {
namespace squirrel {

namespace sprite_class {
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
    class texture_holder& texture_holder = texture_class::get_instance(vm, 2);
    sprite* self = new sprite(texture_holder);
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

sprite&
get_instance(HSQUIRRELVM vm, SQInteger idx) {
  SQUserPointer p;
  {
    SQRESULT result = ::sq_getinstanceup(vm, idx, &p, type_tag());
    if (SQ_FAILED(result)) {
      throw squirrel_error(result);
    }
  }
  sprite* self = reinterpret_cast<sprite*>(p);
  return *self;
}

// TODO: use slots! (_get and _set)

#define DEFINE_PROPERTY_FUNCS(sq_type,                 \
                              sq_push_func,            \
                              sq_get_func,             \
                              cpp_type,                \
                              sq_get_name,             \
                              sq_set_name,             \
                              cpp_get_name,            \
                              cpp_set_name)            \
  SQInteger                                            \
  method_##sq_get_name(HSQUIRRELVM vm) {               \
    try {                                              \
      sprite& self = get_instance(vm, 1);              \
      sq_push_func(vm, self.cpp_get_name());           \
      return 1;                                        \
    } catch (squirrel_error const& e) {                \
      return e.sq_value();                             \
    }                                                  \
  }                                                    \
  SQInteger                                            \
  method_##sq_set_name(HSQUIRRELVM vm) {               \
    try {                                              \
      sprite& self = get_instance(vm, 1);              \
      sq_type value;                                   \
      sq_get_func(vm, 2, &value);                      \
      self.cpp_set_name(static_cast<cpp_type>(value)); \
      HSQOBJECT self_obj;                              \
      ::sq_getstackobj(vm, 1, &self_obj);              \
      ::sq_pushobject(vm, self_obj);                   \
      return 1;                                        \
    } catch (squirrel_error const& e) {                \
      return e.sq_value();                             \
    }                                                  \
  }

#define DEFINE_PROPERTY_FUNCS_INT(name)   \
  DEFINE_PROPERTY_FUNCS(SQInteger,        \
                        ::sq_pushinteger, \
                        ::sq_getinteger,  \
                        int,              \
                        get_##name,       \
                        set_##name,       \
                        name,             \
                        set_##name)

#define DEFINE_PROPERTY_FUNCS_DOUBLE(name) \
  DEFINE_PROPERTY_FUNCS(SQFloat,           \
                        ::sq_pushfloat,    \
                        ::sq_getfloat,     \
                        double,            \
                        get_##name,        \
                        set_##name,        \
                        name,              \
                        set_##name)

#define DEFINE_PROPERTY_FUNCS_BOOL(name) \
  DEFINE_PROPERTY_FUNCS(SQBool,          \
                        ::sq_pushbool,   \
                        ::sq_getbool,    \
                        bool,            \
                        is_##name,       \
                        set_##name,      \
                        is_##name,       \
                        set_##name)

DEFINE_PROPERTY_FUNCS_INT(src_x)
DEFINE_PROPERTY_FUNCS_INT(src_y)
DEFINE_PROPERTY_FUNCS_INT(src_width)
DEFINE_PROPERTY_FUNCS_INT(src_height)
DEFINE_PROPERTY_FUNCS_INT(x)
DEFINE_PROPERTY_FUNCS_INT(y)
DEFINE_PROPERTY_FUNCS_DOUBLE(alpha)
DEFINE_PROPERTY_FUNCS_BOOL(visible)

#undef DEFINE_PROPRETY_FUNCS_BOOL
#undef DEFINE_PROPRETY_FUNCS_DOUBLE
#undef DEFINE_PROPRETY_FUNCS_INT
#undef DEFINE_PROPERTY_FUNCS

SQInteger
metamethod_get(HSQUIRRELVM vm) {
  sprite& self = get_instance(vm, 1);
  
  return 1;
}

}
}

}
}

#endif
