#ifndef EBITEN_SCRIPT_SQUIRREL_GAME_HPP
#define EBITEN_SCRIPT_SQUIRREL_GAME_HPP

#include "ebiten_script/sprite.hpp"
#include "ebiten_script/squirrel/geometry_matrix_funcs.hpp"
#include "ebiten_script/squirrel/texture_factory_funcs.hpp"
#include "ebiten_script/squirrel/texture_funcs.hpp"
#include "ebiten/graphics/graphics_context.hpp"
#include "ebiten/graphics/texture_factory.hpp"
#include "ebiten/noncopyable.hpp"
#include <squirrel.h> 
#include <sqstdio.h> 
#include <sqstdaux.h>
#include <cstdio>
#include <exception>
#include <string>
#include <iostream>

namespace ebiten_script {
namespace squirrel {

class game : private ebiten::noncopyable {
private:
  HSQUIRRELVM const vm_;
  SQInteger top_;
  SQInteger game_idx_;
public:
  game(std::string filename)
    : vm_(::sq_open(1024)),
      game_idx_(0) {
    ::sqstd_seterrorhandlers(this->vm_);
    ::sq_setprintfunc(this->vm_, print_func, error_func);
    // TODO: load the standard library
    this->top_ = ::sq_gettop(this->vm_);
    this->initialize_ebiten_classes();
    ::sq_pushroottable(this->vm_);
    ::sqstd_dofile(this->vm_,
                   _SC(filename.c_str()),
                   SQTrue,
                   SQTrue);
    if (::sq_gettype(this->vm_, -1) == OT_INSTANCE) {
      this->game_idx_ = ::sq_gettop(this->vm_);
    }
  }
  virtual
  ~game() {
    ::sq_settop(this->vm_, this->top_);
    ::sq_close(this->vm_);
  }
  void update(ebiten::graphics::texture_factory& tf) {
    SQInteger const top = ::sq_gettop(this->vm_);
    SQInteger texture_factory_idx;
    {
      ::sq_pushroottable(this->vm_);
      ::sq_pushstring(this->vm_, _SC("ebiten"), -1);
      ::sq_get(this->vm_, -2);
      ::sq_pushstring(this->vm_, _SC("TextureFactory"), -1);
      ::sq_get(this->vm_, -2);
      ::sq_pushroottable(this->vm_); // 'this'
      ::sq_pushuserpointer(this->vm_, &tf);
      ::sq_call(this->vm_, 2, SQTrue, SQTrue);
      texture_factory_idx = ::sq_gettop(this->vm_);
    }
    ::sq_push(this->vm_, this->game_idx_);
    ::sq_pushstring(this->vm_, _SC("update"), -1);
    ::sq_get(this->vm_, -2);
    ::sq_pushroottable(this->vm_); // 'this'
    ::sq_push(this->vm_, texture_factory_idx);
    ::sq_call(this->vm_, 2, SQFalse, SQTrue);
    // TODO: change state of the texture_factory?
    ::sq_settop(this->vm_, top);
  }
  void draw(ebiten::graphics::graphics_context&) {
    this->call_method(this->game_idx_, "draw");
  }
private:
  static void
  print_func(HSQUIRRELVM, SQChar const* s, ...) {
    va_list vl;
    ::va_start(vl, s);
    ::vfprintf(stdout, s, vl);
    ::va_end(vl);
  }
  static void
  error_func(HSQUIRRELVM, SQChar const* s, ...) {
    va_list vl;
    ::va_start(vl, s);
    ::vfprintf(stderr, s, vl);
    ::va_end(vl);
  }
  void
  initialize_ebiten_classes() {
    static std::string const e("ebiten");
    this->create_table(e);
    {
      static std::string const gm("GeometryMatrix");
      this->create_class(e, gm);
      this->create_method(e, gm,
                          "constructor",
                          geometry_matrix_funcs::constructor,
                          "x",
                          false);
      this->create_method(e, gm,
                          "_tostring",
                          geometry_matrix_funcs::meta_tostring,
                          "x",
                          false);
    }
    {
      static std::string const t("TextureFactory");
      this->create_class(e, t);
      this->create_method(e, t,
                          "constructor",
                          texture_factory_funcs::constructor,
                          "xp",
                          false);
      this->create_method(e, t,
                          "_typeof",
                          texture_factory_funcs::meta_typeof,
                          "x",
                          false);
      this->create_method(e, t,
                          "load",
                          texture_factory_funcs::load,
                          "x",
                          false);
      this->create_method(e, t,
                          "create",
                          texture_factory_funcs::create,
                          "x",
                          false);
    }
    {
      static std::string const t("Texture");
      this->create_class(e, t);
      this->create_method(e, t,
                          "constructor",
                          texture_funcs::constructor,
                          "x",
                          false);
    }
  }
  void
  create_table(std::string const& name) {
    SQInteger const top = ::sq_gettop(this->vm_);
    ::sq_pushroottable(this->vm_);
    ::sq_pushstring(this->vm_, _SC(name.c_str()), -1);
    ::sq_newtable(this->vm_);
    ::sq_newslot(this->vm_, -3, SQFalse);
    ::sq_settop(this->vm_, top);
  }
  void
  create_class(std::string const& namespace_name,
               std::string const& class_name) {
    SQInteger const top = ::sq_gettop(this->vm_);
    ::sq_pushroottable(this->vm_);
    ::sq_pushstring(this->vm_, _SC(namespace_name.c_str()), -1);
    ::sq_get(this->vm_, -2);
    ::sq_pushstring(this->vm_, _SC(class_name.c_str()), -1);
    ::sq_newclass(this->vm_, SQFalse);
    ::sq_newslot(this->vm_, -3, SQFalse);
    ::sq_settop(this->vm_, top);
  }
  void
  create_method(std::string const& namespace_name,
                std::string const& class_name,
                std::string const& method_name,
                SQFUNCTION func,
                std::string const& type_mask,
                bool is_static) {
    SQInteger const top = ::sq_gettop(this->vm_);
    ::sq_pushroottable(this->vm_);
    ::sq_pushstring(this->vm_, _SC(namespace_name.c_str()), -1);
    ::sq_get(this->vm_, -2);
    ::sq_pushstring(this->vm_, _SC(class_name.c_str()), -1);
    ::sq_get(this->vm_, -2);
    ::sq_pushstring(this->vm_, _SC(method_name.c_str()), -1);
    ::sq_newclosure(this->vm_, func, 0);
    ::sq_setparamscheck(this->vm_, type_mask.size(), _SC(type_mask.c_str()));
    ::sq_newslot(this->vm_, -3, is_static);
    ::sq_settop(this->vm_, top);
  }
  void
  call_method(SQInteger idx,
              std::string const& method_name) {
    SQInteger const top = ::sq_gettop(this->vm_);
    ::sq_push(this->vm_, idx);
    ::sq_pushstring(this->vm_, method_name.c_str(), -1);
    if (SQ_FAILED(::sq_get(this->vm_, -2))) {
      std::string msg = std::string("method not found: ") + method_name;
      ::sq_throwerror(this->vm_, _SC(msg.c_str()));
      return;
    }
    ::sq_pushroottable(this->vm_); // 'this'
    ::sq_call(this->vm_, 1, SQFalse, SQTrue);
    ::sq_settop(this->vm_, top);
  }
};

}
}

#endif
