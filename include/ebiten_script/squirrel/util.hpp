#ifndef EBITEN_SCRIPT_SQUIRREL_UTIL_HPP
#define EBITEN_SCRIPT_SQUIRREL_UTIL_HPP

#include "ebiten/noncopyable.hpp"
#include <squirrel.h>

namespace ebiten_script {
namespace squirrel {

namespace util {

class stack_restorer : private ebiten::noncopyable {
private:
  HSQUIRRELVM vm_;
  SQInteger const top_;
public:
  stack_restorer(HSQUIRRELVM vm)
    : vm_(vm),
      top_(::sq_gettop(vm)) {
  }
  ~stack_restorer() {
    ::sq_settop(this->vm_, this->top_);
  }
};

// TODO: make class?
namespace {

void
create_function(HSQUIRRELVM const& vm,
                std::string const& namespace_name,
                std::string const& method_name,
                SQFUNCTION const& func,
                std::string const& type_mask) {
  stack_restorer r(vm);
  ::sq_pushroottable(vm);
  ::sq_pushstring(vm, _SC(namespace_name.c_str()), -1);
  ::sq_rawget(vm, -2);
  ::sq_pushstring(vm, _SC(method_name.c_str()), -1);
  ::sq_newclosure(vm, func, 0);
  if (!type_mask.empty()) {
    // TODO: Set even it is empty
    ::sq_setparamscheck(vm, type_mask.size(), _SC(type_mask.c_str()));
  }
  ::sq_newslot(vm, -3, SQFalse);
}

HSQOBJECT
create_class(HSQUIRRELVM const& vm,
             std::string const& namespace_name,
             std::string const& class_name,
             SQUserPointer const& type_tag) {
  stack_restorer r(vm);
  ::sq_pushroottable(vm);
  ::sq_pushstring(vm, _SC(namespace_name.c_str()), -1);
  ::sq_rawget(vm, -2);
  ::sq_pushstring(vm, _SC(class_name.c_str()), -1);
  ::sq_newclass(vm, SQFalse);
  ::sq_settypetag(vm, -1, type_tag);
  HSQOBJECT klass;
  ::sq_getstackobj(vm, -1, &klass);
  ::sq_newslot(vm, -3, SQFalse);
  return klass;
}

void
create_method(HSQUIRRELVM const& vm,
              HSQOBJECT const& klass,
              std::string const& method_name,
              SQFUNCTION const& func,
              std::string const& type_mask,
              bool is_static) {
  stack_restorer r(vm);
  ::sq_pushobject(vm, klass);
  ::sq_pushstring(vm, _SC(method_name.c_str()), -1);
  ::sq_newclosure(vm, func, 0);
  if (!type_mask.empty()) {
    ::sq_setparamscheck(vm, type_mask.size(), _SC(type_mask.c_str()));
  }
  ::sq_newslot(vm, -3, is_static);
}

void
create_variable(HSQUIRRELVM const& vm,
                HSQOBJECT const& klass,
                std::string const& variable_name,
                HSQOBJECT const& value,
                bool is_static) {
  stack_restorer r(vm);
  ::sq_pushobject(vm, klass);
  ::sq_pushstring(vm, _SC(variable_name.c_str()), -1);
  ::sq_pushobject(vm, value);
  ::sq_newslot(vm, -3, is_static);
}

void
create_null_variable(HSQUIRRELVM const& vm,
                     HSQOBJECT const& klass,
                     std::string const& variable_name) {
  stack_restorer r(vm);
  ::sq_pushobject(vm, klass);
  ::sq_pushstring(vm, _SC(variable_name.c_str()), -1);
  ::sq_pushnull(vm);
  ::sq_newslot(vm, -3, SQFalse);
}

void
push_values(HSQUIRRELVM const&) {
}

template<class... Args>
void
push_values(HSQUIRRELVM const& vm, int value, Args... args) {
  ::sq_pushinteger(vm, value);
  push_values(vm, args...);
}

template<class... Args>
void
push_values(HSQUIRRELVM const& vm, float value, Args... args) {
  ::sq_pushfloat(vm, value);
  push_values(vm, args...);
}

template<class... Args>
void
push_values(HSQUIRRELVM const& vm, double value, Args... args) {
  ::sq_pushfloat(vm, value);
  push_values(vm, args...);
}

template<class... Args>
void
push_values(HSQUIRRELVM const& vm, SQUserPointer const& value, Args... args) {
  ::sq_pushuserpointer(vm, value);
  push_values(vm, args...);
}

template<class... Args>
void
push_values(HSQUIRRELVM const& vm, HSQOBJECT const& value, Args... args) {
  ::sq_pushobject(vm, value);
  push_values(vm, args...);
}

template<class... Args>
HSQOBJECT
call(HSQUIRRELVM const& vm,
     HSQOBJECT const& receiver,
     std::string const& method_name,
     bool return_value,
     Args... args) {
  stack_restorer r(vm);
  ::sq_pushobject(vm, receiver);
  ::sq_pushstring(vm, _SC(method_name.c_str()), -1);
  ::sq_rawget(vm, -2);
  if (::sq_gettype(vm, -2) == OT_INSTANCE) {
    ::sq_pushobject(vm, receiver);
  } else {
    ::sq_pushroottable(vm);
  }
  push_values(vm, args...);
  if (SQ_FAILED(::sq_call(vm, sizeof...(Args) + 1, return_value, SQTrue))) {
    throw squirrel_error(vm);
  }
  if (!return_value) {
    ::sq_pushnull(vm);
  }
  HSQOBJECT return_obj;
  ::sq_getstackobj(vm, -1, &return_obj);
  ::sq_addref(vm, &return_obj);
  return return_obj;
}

}
}

}
}

#endif
