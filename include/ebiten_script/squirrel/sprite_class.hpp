#ifndef EBITEN_SCRIPT_SQUIRREL_SPRITE_CLASS_HPP
#define EBITEN_SCRIPT_SQUIRREL_SPRITE_CLASS_HPP

#include "ebiten_script/squirrel/squirrel_error.hpp"
#include "ebiten_script/squirrel/texture_class.hpp"
#include "ebiten_script/squirrel/util.hpp"
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
      // TODO: use ::sq_throwobject(vm); ?
      throw squirrel_error(result);
    }
  }
  sprite* self = reinterpret_cast<sprite*>(p);
  return *self;
}

SQInteger
metamethod_get(HSQUIRRELVM vm) {
  try {
    sprite& self = get_instance(vm, 1);
    SQChar const* method_name_p;
    ::sq_getstring(vm, 2, &method_name_p);
    std::string method_name(method_name_p);
    if (method_name == "srcX") {
      ::sq_pushinteger(vm, self.src_x());
    } else if (method_name == "srcY") {
      ::sq_pushinteger(vm, self.src_y());
    } else if (method_name == "srcWidth") {
      ::sq_pushinteger(vm, self.src_width());
    } else if (method_name == "srcHeight") {
      ::sq_pushinteger(vm, self.src_height());
    } else if (method_name == "x") {
      ::sq_pushinteger(vm, self.x());
    } else if (method_name == "y") {
      ::sq_pushinteger(vm, self.y());
    } else if (method_name == "alpha") {
      ::sq_pushfloat(vm, self.alpha());
    } else if (method_name == "isVisible") {
      ::sq_pushbool(vm, self.is_visible());
    } else {
      std::string msg = "the index '" + method_name + "' does not exist";
      return ::sq_throwerror(vm, _SC(msg.c_str()));
    }
    return 1;
  } catch (squirrel_error const& e) {
    return e.sq_value();
  }
}

int
get_int_value(HSQUIRRELVM vm, HSQOBJECT& obj) {
  if (!sq_isnumeric(obj)) {
    throw squirrel_error(vm,
                         "invalid param type ; expected: 'integer|float'");
  }
  return static_cast<int>(::sq_objtointeger(&obj));
}

double
get_float_value(HSQUIRRELVM vm, HSQOBJECT& obj) {
  if (!sq_isnumeric(obj)) {
    throw squirrel_error(vm,
                         "invalid param type ; expected: 'integer|float'");
  }
  return static_cast<double>(::sq_objtofloat(&obj));
}

double
get_bool_value(HSQUIRRELVM vm, HSQOBJECT& obj) {
  if (!sq_isbool(obj)) {
    throw squirrel_error(vm,
                         "invalid param type ; expected: 'bool'");
  }
  return static_cast<bool>(::sq_objtobool(&obj));
}

SQInteger
metamethod_set(HSQUIRRELVM vm) {
  try {
    sprite& self = get_instance(vm, 1);
    SQChar const* method_name_p;
    ::sq_getstring(vm, 2, &method_name_p);
    std::string method_name(method_name_p);
    HSQOBJECT value;
    ::sq_getstackobj(vm, 3, &value);
    if (method_name == "srcX") {
      self.set_src_x(get_int_value(vm, value));
    } else if (method_name == "srcY") {
      self.set_src_y(get_int_value(vm, value));
    } else if (method_name == "srcWidth") {
      self.set_src_width(get_int_value(vm, value));
    } else if (method_name == "srcHeight") {
      self.set_src_height(get_int_value(vm, value));
    } else if (method_name == "x") {
      self.set_x(get_int_value(vm, value));
    } else if (method_name == "y") {
      self.set_y(get_int_value(vm, value));
    } else if (method_name == "alpha") {
      self.set_alpha(get_float_value(vm, value));
    } else if (method_name == "isVisible") {
      self.set_visible(get_bool_value(vm, value));
    } else {
      std::string msg = "the index '" + method_name + "' does not exist";
      return ::sq_throwerror(vm, _SC(msg.c_str()));
    }
    return 0;
  } catch (squirrel_error const& e) {
    return e.sq_value();
  }
}

void
initialize(HSQUIRRELVM vm) {
  HSQOBJECT klass = util::create_class(vm, "ebiten", "Sprite", type_tag());
  util::create_method(vm, klass, "constructor", method_constructor,
                      "xx", false);
  util::create_method(vm, klass, "_get", metamethod_get,
                      "xs", false);
  util::create_method(vm, klass, "_set", metamethod_set,
                      "xs.", false);
}

}
}

}
}

#endif
