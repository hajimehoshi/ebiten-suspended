#ifndef EBITEN_SCRIPT_SQUIRREL_TEXTURE_FUNCS_HPP
#define EBITEN_SCRIPT_SQUIRREL_TEXTURE_FUNCS_HPP

#include "ebiten/graphics/texture_factory.hpp"
#include "ebiten/noncopyable.hpp"
#include "ebiten_script/texture_holder.hpp"
#include "ebiten_script/texture_command.hpp"
#include <squirrel.h> 
#include <sqstdio.h> 
#include <sqstdaux.h>
#include <cassert>
#include <fstream>
#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace ebiten_script {
namespace squirrel {

class texture_holders : private ebiten::noncopyable {
public:
  typedef std::size_t key_type;
private:
  std::unordered_map<key_type, std::pair<HSQUIRRELVM, std::shared_ptr<texture_holder> > > set_;
  key_type unique_number_;
public:
  texture_holders()
    : unique_number_(0) {
  }
  key_type
  insert(HSQUIRRELVM vm, std::shared_ptr<texture_holder> const& t) {
    key_type key = this->unique_number_;
    std::pair<HSQUIRRELVM, std::shared_ptr<texture_holder> > value(vm, t);
    this->set_.emplace(key, std::move(value));
    ++this->unique_number_;
    return key;
  }
  texture_holder&
  get(key_type const& key) {
    return *this->set_[key].second;
  }
  void
  remove(key_type const& key) {
    this->set_.erase(key);
  }
  void
  instantiate(HSQUIRRELVM vm, ebiten::graphics::texture_factory& tf) {
    for (auto& p : this->set_) {
      if (vm != p.second.first) {
        continue;
      }
      std::shared_ptr<texture_holder>& t = p.second.second;
      if (t->ebiten_texture()) {
        continue;
      }
      t->instantiate(tf);
    }
  }
  void
  flush_drawing_commands(HSQUIRRELVM vm, ebiten::graphics::graphics_context& g) {
    for (auto& p : this->set_) {
      if (vm != p.second.first) {
        continue;
      }
      std::shared_ptr<texture_holder>& t = p.second.second;
      assert(static_cast<bool>(t->ebiten_texture()));
      //t->flush_drawing_commands(g);
    }
  }
  void
  add_command(HSQUIRRELVM) {
  }
};

class texture_funcs {
private:
  static texture_holders texture_holders_;
public:
  texture_funcs() = delete;
  texture_funcs(texture_funcs const&) = delete;
  texture_funcs& operator=(texture_funcs const&) = delete;
  static void
  instantiate(HSQUIRRELVM vm, ebiten::graphics::texture_factory& tf) {
    texture_holders_.instantiate(vm, tf);
  }
  static void
  flush_drawing_commands(HSQUIRRELVM vm, ebiten::graphics::graphics_context& g) {
    texture_holders_.flush_drawing_commands(vm, g);
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
    std::shared_ptr<texture_holder> t(new texture_holder());
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
      t->set_path(path);
    }
    if (n_args == 3) {
      SQInteger width, height;
      ::sq_getinteger(vm, 2, &width);
      ::sq_getinteger(vm, 3, &height);
      t->set_size(width, height);
    }
    texture_holders::key_type key = texture_holders_.insert(vm, t);
    ::sq_setinstanceup(vm, 1, reinterpret_cast<SQUserPointer>(key));
    ::sq_setreleasehook(vm, 1, releasehook);
    return 0;
  }
  static SQInteger
  releasehook(SQUserPointer p, SQInteger) {
    texture_holders::key_type key = reinterpret_cast<texture_holders::key_type>(p);
    texture_holders_.remove(key);
    return 1;
  }
  static SQInteger
  method_is_created(HSQUIRRELVM vm) {
    SQUserPointer p;
    ::sq_getinstanceup(vm, 1, &p, 0);
    texture_holders::key_type key = reinterpret_cast<texture_holders::key_type>(p);
    texture_holder const& self = texture_holders_.get(key);
    ::sq_pushbool(vm, static_cast<bool>(self.ebiten_texture()));
    return 1;
  }
  static SQInteger
  method_get_width(HSQUIRRELVM vm) {
    SQUserPointer p;
    ::sq_getinstanceup(vm, 1, &p, 0);
    texture_holders::key_type key = reinterpret_cast<texture_holders::key_type>(p);
    texture_holder const& self = texture_holders_.get(key);
    ::sq_pushinteger(vm, self.ebiten_texture().width());
    return 1;
  }
  static SQInteger
  method_get_height(HSQUIRRELVM vm) {
    SQUserPointer p;
    ::sq_getinstanceup(vm, 1, &p, 0);
    texture_holders::key_type key = reinterpret_cast<texture_holders::key_type>(p);
    texture_holder const& self = texture_holders_.get(key);
    ::sq_pushinteger(vm, self.ebiten_texture().height());
    return 1;
  }
  static SQInteger
  method_clear(HSQUIRRELVM vm) {
    SQUserPointer p;
    ::sq_getinstanceup(vm, 1, &p, 0);
    //texture_holders::key_type key = reinterpret_cast<texture_holders::key_type>(p);
    //texture_holders_.add_command(vm, key, );
    return 0;
  }
};

texture_holders texture_funcs::texture_holders_;

}
}

#endif
