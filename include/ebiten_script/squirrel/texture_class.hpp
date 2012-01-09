#ifndef EBITEN_SCRIPT_SQUIRREL_TEXTURE_CLASS_HPP
#define EBITEN_SCRIPT_SQUIRREL_TEXTURE_CLASS_HPP

#include "ebiten_script/squirrel/squirrel_error.hpp"
#include "ebiten_script/squirrel/util.hpp"
#include "ebiten_script/sprite.hpp"
#include "ebiten_script/texture_holder.hpp"
#include "ebiten_script/texture_holders.hpp"
#include "ebiten/graphics/graphics_context.hpp"
#include "ebiten/graphics/texture_factory.hpp"
#include "ebiten/resources.hpp"
#include <squirrel.h>
#include <cassert>
#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>

namespace ebiten_script {
namespace squirrel {

namespace sprite_class {
namespace {

sprite&
get_instance(HSQUIRRELVM, SQInteger);

}

}

class texture_class {
private:
  static std::unordered_map<HSQUIRRELVM, texture_holders> vm_to_texture_holders_;
  typedef std::pair<texture_holders::key_type, HSQUIRRELVM> key_vm_type;
public:
  texture_class() = delete;
  texture_class(texture_class const&) = delete;
  texture_class& operator=(texture_class const&) = delete;
  static void
  instantiate(HSQUIRRELVM vm, ebiten::graphics::texture_factory& tf) {
    get_texture_holders(vm).instantiate(tf);
  }
  static void
  flush_texture_commands(HSQUIRRELVM vm, ebiten::graphics::graphics_context& g) {
    get_texture_holders(vm).flush_texture_commands(g);
  }
  static SQUserPointer
  type_tag() {
    return reinterpret_cast<SQUserPointer>(method_constructor);
  }
  static SQInteger
  method_constructor(HSQUIRRELVM vm) {
    try {
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
        SQChar const* c_path;
        ::sq_getstring(vm, 2, &c_path);
        std::string const& path = ebiten::get_resource_path(c_path);
        if (!std::ifstream(path)) {
          std::string error_message = std::string("file not found: ") + c_path;
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
    } catch (squirrel_error const& e) {
      return e.sq_value();
    }
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
  static texture_holder&
  get_instance(HSQUIRRELVM vm, SQInteger idx) {
    SQUserPointer p;
    if (SQ_FAILED(::sq_getinstanceup(vm, idx, &p, type_tag()))) {
      throw squirrel_error(vm);
    }
    texture_holders::key_type key = reinterpret_cast<key_vm_type*>(p)->first;
    return get_texture_holders(vm).get(key);
  }
  static SQInteger
  metamethod_get(HSQUIRRELVM vm) {
    try {
      texture_holder& self = get_instance(vm, 1);
      SQChar const* method_name_p;
      ::sq_getstring(vm, 2, &method_name_p);
      std::string method_name(method_name_p);
      if (method_name == "isCreated") {
        ::sq_pushbool(vm, static_cast<bool>(self.is_instantiated()));
        return 1;
      }
      if (!self.is_instantiated()) {
        return ::sq_throwerror(vm, "the texture is not created yet");
      }
      if (method_name == "width") {
        ::sq_pushinteger(vm, self.ebiten_texture().width());
      } else if (method_name == "height") {
        ::sq_pushinteger(vm, self.ebiten_texture().height());
      } else {
        std::string msg = "the index '" + method_name + "' does not exist";
        return ::sq_throwerror(vm, _SC(msg.c_str()));
      }
      return 1;
    } catch (squirrel_error const& e) {
      return e.sq_value();
    }
  }
  static SQInteger
  method_clear(HSQUIRRELVM vm) {
    try {
      texture_holder& self = get_instance(vm, 1);
      std::unique_ptr<texture_command> command(new texture_command_clear(self));
      get_texture_holders(vm).add_texture_command(command);
      return 0;
    } catch (squirrel_error const& e) {
      return e.sq_value();
    }
  }
  static SQInteger
  method_draw_rect(HSQUIRRELVM vm) {
    try {
      texture_holder& self = get_instance(vm, 1);
      SQInteger x, y, width, height, red, green, blue, alpha;
      ::sq_getinteger(vm, 2, &x);
      ::sq_getinteger(vm, 3, &y);
      ::sq_getinteger(vm, 4, &width);
      ::sq_getinteger(vm, 5, &height);
      ::sq_getinteger(vm, 6, &red);
      ::sq_getinteger(vm, 7, &green);
      ::sq_getinteger(vm, 8, &blue);
      ::sq_getinteger(vm, 9, &alpha);
      typedef texture_command_draw_rect tcdr;
      std::unique_ptr<texture_command> command(new tcdr(self,
                                                        x, y, width, height,
                                                        red, green, blue, alpha));
      get_texture_holders(vm).add_texture_command(command);
      return 0;
    } catch (squirrel_error const& e) {
      return e.sq_value();
    }
  }
  static SQInteger
  method_draw_sprite(HSQUIRRELVM vm) {
    try {
      texture_holder& self = get_instance(vm, 1);
      class sprite& sprite = sprite_class::get_instance(vm, 2);
      typedef texture_command_draw_sprite tcds;
      std::unique_ptr<texture_command> command(new tcds(self, sprite));
      get_texture_holders(vm).add_texture_command(command);
      return 0;
    } catch (squirrel_error const& e) {
      return e.sq_value();
    }
  }
  static SQInteger
  method_set_texture(HSQUIRRELVM vm) {
    try {
      texture_holder& self = get_instance(vm, 1);
      SQUserPointer p_texture;
      ::sq_getuserpointer(vm, 2, &p_texture);
      ebiten::graphics::texture* texture =
        reinterpret_cast<ebiten::graphics::texture*>(p_texture);
      self.set_ebiten_texture(texture);
      return 0;
    } catch (squirrel_error const& e) {
      return e.sq_value();
    }
  }
  static void
  initialize(HSQUIRRELVM vm) {
    HSQOBJECT klass = util::create_class(vm, "ebiten", "Texture", type_tag());
    util::create_method(vm, klass, "constructor", method_constructor,
                        "", false);
    util::create_method(vm, klass, "_get", metamethod_get,
                        "xs", false);
    util::create_method(vm, klass, "clear", method_clear,
                        "x", false);
    util::create_method(vm, klass, "drawRect", method_draw_rect,
                        "xnnnnnnnn", false);
    util::create_method(vm, klass, "drawSprite", method_draw_sprite,
                        "xx", false);
    util::create_method(vm, klass, "setTexture_", method_set_texture,
                        "xp", false);
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

std::unordered_map<HSQUIRRELVM, texture_holders> texture_class::vm_to_texture_holders_;

}
}

#endif
