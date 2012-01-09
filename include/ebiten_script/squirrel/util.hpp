#ifndef EBITEN_SCRIPT_SQUIRREL_UTIL_HPP
#define EBITEN_SCRIPT_SQUIRREL_UTIL_HPP

namespace ebiten_script {
namespace squirrel {

namespace util {
namespace {

void
create_function(HSQUIRRELVM const& vm,
                std::string const& namespace_name,
                std::string const& method_name,
                SQFUNCTION const& func,
                std::string const& type_mask) {
  SQInteger const top = ::sq_gettop(vm);
  ::sq_pushroottable(vm);
  ::sq_pushstring(vm, _SC(namespace_name.c_str()), -1);
  ::sq_get(vm, -2);
  ::sq_pushstring(vm, _SC(method_name.c_str()), -1);
  ::sq_newclosure(vm, func, 0);
  if (!type_mask.empty()) {
    // TODO: Set even it is empty
    ::sq_setparamscheck(vm, type_mask.size(), _SC(type_mask.c_str()));
  }
  ::sq_newslot(vm, -3, SQFalse);
  ::sq_settop(vm, top);
}

HSQOBJECT
create_class(HSQUIRRELVM const& vm,
             std::string const& namespace_name,
             std::string const& class_name,
             SQUserPointer const& type_tag) {
  SQInteger const top = ::sq_gettop(vm);
  ::sq_pushroottable(vm);
  ::sq_pushstring(vm, _SC(namespace_name.c_str()), -1);
  ::sq_get(vm, -2);
  ::sq_pushstring(vm, _SC(class_name.c_str()), -1);
  ::sq_newclass(vm, SQFalse);
  ::sq_settypetag(vm, -1, type_tag);
  HSQOBJECT klass;
  ::sq_getstackobj(vm, -1, &klass);
  ::sq_newslot(vm, -3, SQFalse);
  ::sq_settop(vm, top);
  return klass;
}

void
create_method(HSQUIRRELVM const& vm,
              HSQOBJECT const& klass,
              std::string const& method_name,
              SQFUNCTION const& func,
              std::string const& type_mask,
              bool is_static) {
  SQInteger const top = ::sq_gettop(vm);
  ::sq_pushobject(vm, klass);
  ::sq_pushstring(vm, _SC(method_name.c_str()), -1);
  ::sq_newclosure(vm, func, 0);
  if (!type_mask.empty()) {
    ::sq_setparamscheck(vm, type_mask.size(), _SC(type_mask.c_str()));
  }
  ::sq_newslot(vm, -3, is_static);
  ::sq_settop(vm, top);
}

void
create_variable(HSQUIRRELVM const& vm,
                HSQOBJECT const& klass,
                std::string const& variable_name,
                HSQOBJECT const& value,
                bool is_static) {
  SQInteger const top = ::sq_gettop(vm);
  ::sq_pushobject(vm, klass);
  ::sq_pushstring(vm, _SC(variable_name.c_str()), -1);
  ::sq_pushobject(vm, value);
  ::sq_newslot(vm, -3, is_static);
  ::sq_settop(vm, top);
}

}
}

}
}

#endif
