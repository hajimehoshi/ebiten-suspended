#ifndef EBITEN_SCRIPT_SQUIRREL_TEXTURE_FUNCS_HPP
#define EBITEN_SCRIPT_SQUIRREL_TEXTURE_FUNCS_HPP

#include "ebiten/graphics/texture_factory.hpp"
#include "ebiten/noncopyable.hpp"
#include "ebiten_script/texture.hpp"
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

class textures : private ebiten::noncopyable {
public:
  typedef std::size_t key_type;
private:
  std::unordered_map<key_type, std::pair<HSQUIRRELVM, std::shared_ptr<texture> > > set_;
  key_type unique_number_;
public:
  textures()
    : unique_number_(0) {
  }
  key_type
  insert(HSQUIRRELVM vm, std::shared_ptr<texture> const& t) {
    key_type key = this->unique_number_;
    std::pair<HSQUIRRELVM, std::shared_ptr<texture> > value(vm, t);
    this->set_.emplace(key, std::move(value));
    ++this->unique_number_;
    return key;
  }
  texture&
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
      std::shared_ptr<texture>& t = p.second.second;
      if (t->is_created()) {
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
      std::shared_ptr<texture>& t = p.second.second;
      assert(t->is_created());
      t->flush_drawing_commands(g);
    }
  }
};

class texture_funcs {
private:
  static textures textures_;
public:
  texture_funcs() = delete;
  texture_funcs(texture_funcs const&) = delete;
  texture_funcs& operator=(texture_funcs const&) = delete;
  static void
  instantiate(HSQUIRRELVM vm, ebiten::graphics::texture_factory& tf) {
    textures_.instantiate(vm, tf);
  }
  static void
  flush_drawing_commands(HSQUIRRELVM vm, ebiten::graphics::graphics_context& g) {
    textures_.flush_drawing_commands(vm, g);
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
    std::shared_ptr<texture> t(new texture());
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
    textures::key_type key = textures_.insert(vm, t);
    ::sq_setinstanceup(vm, 1, reinterpret_cast<SQUserPointer>(key));
    ::sq_setreleasehook(vm, 1, releasehook);
    return 0;
  }
  static SQInteger
  releasehook(SQUserPointer p, SQInteger) {
    textures::key_type key = reinterpret_cast<textures::key_type>(p);
    textures_.remove(key);
    return 1;
  }
  static SQInteger
  method_is_created(HSQUIRRELVM vm) {
    SQUserPointer p;
    ::sq_getinstanceup(vm, 1, &p, 0);
    textures::key_type key = reinterpret_cast<textures::key_type>(p);
    texture const& self = textures_.get(key);
    ::sq_pushbool(vm, self.is_created());
    return 1;
  }
  static SQInteger
  method_get_width(HSQUIRRELVM vm) {
    SQUserPointer p;
    ::sq_getinstanceup(vm, 1, &p, 0);
    textures::key_type key = reinterpret_cast<textures::key_type>(p);
    texture const& self = textures_.get(key);
    ::sq_pushinteger(vm, self.width());
    return 1;
  }
  static SQInteger
  method_get_height(HSQUIRRELVM vm) {
    SQUserPointer p;
    ::sq_getinstanceup(vm, 1, &p, 0);
    textures::key_type key = reinterpret_cast<textures::key_type>(p);
    texture const& self = textures_.get(key);
    ::sq_pushinteger(vm, self.height());
    return 1;
  }
};

textures texture_funcs::textures_;

}
}

#endif
