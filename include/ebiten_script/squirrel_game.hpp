#ifndef EBITEN_SCRIPT_SQUIRREL_GAME_HPP
#define EBITEN_SCRIPT_SQUIRREL_GAME_HPP

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

class squirrel_game : private ebiten::noncopyable {
private:
  HSQUIRRELVM const vm_;
  SQInteger top_;
  SQInteger game_idx_;
public:
  squirrel_game(std::string filename)
    : vm_(::sq_open(1024)),
      game_idx_(0) {
    ::sqstd_seterrorhandlers(this->vm_);
    ::sq_setprintfunc(this->vm_, print_func, error_func);
    this->top_ = ::sq_gettop(this->vm_);
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
  ~squirrel_game() {
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
  call_method(SQInteger idx,
              std::string const& method_name) {
    SQInteger const top = ::sq_gettop(this->vm_);
    ::sq_pushstring(this->vm_, method_name.c_str(), -1);
    if (SQ_FAILED(::sq_get(this->vm_, idx))) {
      throw std::runtime_error(std::string("method not found: ") + method_name);
    }
    ::sq_pushroottable(this->vm_); // 'this'
    ::sq_call(this->vm_, 1, SQFalse, SQFalse);
    ::sq_settop(this->vm_, top);
  }
};

}

#endif
