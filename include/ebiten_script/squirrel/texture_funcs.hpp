#ifndef EBITEN_SCRIPT_SQUIRREL_TEXTURE_FUNCS_HPP
#define EBITEN_SCRIPT_SQUIRREL_TEXTURE_FUNCS_HPP

#include "ebiten_script/texture_holder.hpp"
#include "ebiten_script/texture_holders.hpp"
#include "ebiten/graphics/graphics_context.hpp"
#include "ebiten/graphics/texture_factory.hpp"
#include "ebiten/noncopyable.hpp"
#include <squirrel.h> 
#include <sqstdio.h> 
#include <sqstdaux.h>
#include <cassert>
#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>

namespace ebiten_script {
namespace squirrel {

class texture_funcs {
private:
  static std::unordered_map<HSQUIRRELVM, texture_holders> vm_to_texture_holders_;
  typedef std::pair<texture_holders::key_type, HSQUIRRELVM> key_vm_type;
public:
  texture_funcs() = delete;
  texture_funcs(texture_funcs const&) = delete;
  texture_funcs& operator=(texture_funcs const&) = delete;
  static void
  instantiate(HSQUIRRELVM vm, ebiten::graphics::texture_factory& tf) {
    get_texture_holders(vm).instantiate(tf);
  }
  static void
  flush_texture_commands(HSQUIRRELVM vm, ebiten::graphics::graphics_context& g) {
    get_texture_holders(vm).flush_texture_commands(g);
  }
  static SQInteger
  method_constructor(HSQUIRRELVM vm) {
    SQInteger n_args = ::sq_gettop(vm);
    if (n_args < 2) {
      return ::sq_throwerror(vm, _SC("too few arguments"));
    }
    if (3 < n_args) {
      return ::sq_throwerror(vm, _SC("too many arguments"));
    }
    texture_holders::key_type key;
    if (n_args == 2) {
      if (::sq_gettype(vm, 2) != OT_STRING) {
        return ::sq_throwerror(vm, _SC("invalid argument type"));
      }
      SQChar const* path;
      ::sq_getstring(vm, 2, &path);
      std::ifstream file(path);
      if (!file) {
        std::string error_message = std::string("file not found: ") + path;
        return ::sq_throwerror(vm, _SC(error_message.c_str()));
      }
      key = get_texture_holders(vm).insert(path);
    }
    if (n_args == 3) {
      SQInteger width, height;
      ::sq_getinteger(vm, 2, &width);
      ::sq_getinteger(vm, 3, &height);
      key = get_texture_holders(vm).insert(width, height);
    }
    auto p = new key_vm_type(key, vm);
    ::sq_setinstanceup(vm, 1, reinterpret_cast<SQUserPointer>(p));
    ::sq_setreleasehook(vm, 1, releasehook);
    return 0;
  }
  static SQInteger
  releasehook(SQUserPointer p, SQInteger) {
    key_vm_type* key_vm = reinterpret_cast<key_vm_type*>(p);
    texture_holders::key_type key = key_vm->first;
    HSQUIRRELVM vm                = key_vm->second;
    get_texture_holders(vm).remove(key);
    delete key_vm;
    return 1;
  }
  static SQInteger
  method_is_created(HSQUIRRELVM vm) {
    SQUserPointer p;
    ::sq_getinstanceup(vm, 1, &p, 0);
    texture_holders::key_type key = reinterpret_cast<key_vm_type*>(p)->first;
    texture_holder const& self = get_texture_holders(vm).get(key);
    ::sq_pushbool(vm, static_cast<bool>(self.ebiten_texture()));
    return 1;
  }
  static SQInteger
  method_get_width(HSQUIRRELVM vm) {
    SQUserPointer p;
    ::sq_getinstanceup(vm, 1, &p, 0);
    texture_holders::key_type key = reinterpret_cast<key_vm_type*>(p)->first;
    texture_holder const& self = get_texture_holders(vm).get(key);
    ::sq_pushinteger(vm, self.ebiten_texture().width());
    return 1;
  }
  static SQInteger
  method_get_height(HSQUIRRELVM vm) {
    SQUserPointer p;
    ::sq_getinstanceup(vm, 1, &p, 0);
    texture_holders::key_type key = reinterpret_cast<key_vm_type*>(p)->first;
    texture_holder const& self = get_texture_holders(vm).get(key);
    ::sq_pushinteger(vm, self.ebiten_texture().height());
    return 1;
  }
  static SQInteger
  method_clear(HSQUIRRELVM vm) {
    SQUserPointer p;
    ::sq_getinstanceup(vm, 1, &p, 0);
    //texture_holders::key_type key = reinterpret_cast<>(p);
    //texture_holders_.add_command(vm, key, );
    return 0;
  }
  static SQInteger
  method_set_texture(HSQUIRRELVM vm) {
    SQUserPointer p;
    ::sq_getinstanceup(vm, 1, &p, 0);
    texture_holders::key_type key = reinterpret_cast<key_vm_type*>(p)->first;
    texture_holder& self = get_texture_holders(vm).get(key);
    SQUserPointer pTexture;
    ::sq_getuserpointer(vm, 2, &pTexture);
    ebiten::graphics::texture texture =
      *reinterpret_cast<ebiten::graphics::texture*>(pTexture);
    self.set_ebiten_texture(texture);
    return 0;
  }
private:
  static texture_holders&
  get_texture_holders(HSQUIRRELVM vm) {
    auto it = vm_to_texture_holders_.find(vm);
    if (it == vm_to_texture_holders_.end()) {
      vm_to_texture_holders_.emplace(vm, std::move(texture_holders()));
    }
    return vm_to_texture_holders_[vm];
  }
};

std::unordered_map<HSQUIRRELVM, texture_holders> texture_funcs::vm_to_texture_holders_;

}
}

#endif
