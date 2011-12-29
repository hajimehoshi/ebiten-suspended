#ifndef EBITEN_SCRIPT_SQUIRREL_GAME_HPP
#define EBITEN_SCRIPT_SQUIRREL_GAME_HPP

#include "ebiten_script/squirrel/geometry_matrix_funcs.hpp"
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
  void update(ebiten::graphics::texture_factory&) {
    this->call_method(this->game_idx_, "update");
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
    {
      static std::string const gm("GeometryMatrix");
      this->create_class(e);
      this->create_inner_class(e, gm);
      this->create_method(e, gm,
                          "constructor",
                          geometry_matrix_funcs::constructor,
                          false);
      this->create_method(e, gm,
                          "_tostring",
                          geometry_matrix_funcs::_tostring,
                          false);
    }
  }
  void
  create_class(std::string const& class_name) {
    SQInteger const top = ::sq_gettop(this->vm_);
    ::sq_pushroottable(this->vm_);
    ::sq_pushstring(this->vm_, _SC(class_name.c_str()), -1);
    ::sq_newclass(this->vm_, SQFalse);
    ::sq_newslot(this->vm_, -3, SQFalse);
    ::sq_settop(this->vm_, top);
  }
  void
  create_inner_class(std::string const& outer_class_name,
                     std::string const& inner_class_name) {
    SQInteger const top = ::sq_gettop(this->vm_);
    ::sq_pushroottable(this->vm_);
    ::sq_pushstring(this->vm_, _SC(outer_class_name.c_str()), -1);
    ::sq_get(this->vm_, -2);
    ::sq_pushstring(this->vm_, _SC(inner_class_name.c_str()), -1);
    ::sq_newclass(this->vm_, SQFalse);
    ::sq_newslot(this->vm_, -3, SQFalse);
    ::sq_settop(this->vm_, top);
  }
  void
  create_method(std::string const& outer_class_name,
                std::string const& inner_class_name,
                std::string const& method_name,
                SQFUNCTION func,
                bool is_static) {
    SQInteger const top = ::sq_gettop(this->vm_);
    ::sq_pushroottable(this->vm_);
    ::sq_pushstring(this->vm_, _SC(outer_class_name.c_str()), -1);
    ::sq_get(this->vm_, -2);
    ::sq_pushstring(this->vm_, _SC(inner_class_name.c_str()), -1);
    ::sq_get(this->vm_, -2);
    ::sq_pushstring(this->vm_, _SC(method_name.c_str()), -1);
    ::sq_newclosure(this->vm_, func, 0);
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
    } else {
      ::sq_pushroottable(this->vm_); // 'this'
      ::sq_call(this->vm_, 1, SQFalse, SQFalse);
    }
    ::sq_settop(this->vm_, top);
  }
};

}
}

#endif
