#ifndef EBITEN_SCRIPT_SQUIRREL_GAME_HPP
#define EBITEN_SCRIPT_SQUIRREL_GAME_HPP

#include "ebiten_script/squirrel/geometry_matrix_class.hpp"
#include "ebiten_script/squirrel/input_class.hpp"
#include "ebiten_script/squirrel/texture_class.hpp"
#include "ebiten_script/squirrel/system_class.hpp"
#include "ebiten/graphics/graphics_context.hpp"
#include "ebiten/graphics/texture_factory.hpp"
#include "ebiten/noncopyable.hpp"
#include <squirrel.h>
#include <sqstdaux.h>
#include <sqstdblob.h>
#include <sqstdio.h>
#include <sqstdmath.h>
#include <sqstdstring.h>
#include <sqstdsystem.h>
#include <cstdio>
#include <exception>
#include <string>
#include <iostream>

namespace ebiten_script {
namespace squirrel {

class game : private ebiten::noncopyable {
private:
  HSQUIRRELVM const vm_;
  HSQOBJECT input_;
  HSQOBJECT system_;
  HSQOBJECT game_;
  bool is_terminated_;
  std::function<void()> terminated_handler_;
public:
  game(std::string filename)
    : vm_(::sq_open(1024)),
      is_terminated_(false) {
    ::sq_setforeignptr(this->vm_, reinterpret_cast<SQUserPointer*>(this));
    ::sqstd_seterrorhandlers(this->vm_);
    ::sq_setprintfunc(this->vm_, print_func, error_func);
    {
      /*
       * [Squirrel]
       * (Load standard libraries)
       */
      util::stack_restorer r(this->vm_);
      ::sq_pushroottable(this->vm_);
      ::sqstd_register_bloblib(this->vm_);
      ::sqstd_register_iolib(this->vm_);
      ::sqstd_register_mathlib(this->vm_);
      ::sqstd_register_stringlib(this->vm_);
      ::sqstd_register_systemlib(this->vm_);
    }
    {
      util::stack_restorer r(this->vm_);
      ::sq_pushroottable(this->vm_);
      this->initialize_ebiten_classes();
    }
    {
      /*
       * [Squirrel]
       * input_ = ::ebiten.Input()
       */
      util::stack_restorer r(this->vm_);
      ::sq_pushroottable(this->vm_);
      ::sq_pushstring(this->vm_, _SC("ebiten"), -1);
      ::sq_get(this->vm_, -2);
      ::sq_pushstring(this->vm_, _SC("Input"), -1);
      ::sq_get(this->vm_, -2);
      ::sq_pushroottable(this->vm_);
      if (SQ_FAILED(::sq_call(this->vm_, 1, SQTrue, SQTrue))) {
        throw squirrel_error(this->vm_);
      }
      if (SQ_FAILED(::sq_getstackobj(this->vm_, -1, &this->input_))) {
        throw squirrel_error(this->vm_);
      }
      ::sq_addref(this->vm_, &this->input_);
    }
    {
      /*
       * [Squirrel]
       * system_ = ::ebiten.System(input_)
       */
      util::stack_restorer r(this->vm_);
      ::sq_pushroottable(this->vm_);
      ::sq_pushstring(this->vm_, _SC("ebiten"), -1);
      ::sq_get(this->vm_, -2);
      ::sq_pushstring(this->vm_, _SC("System"), -1);
      ::sq_get(this->vm_, -2);
      ::sq_pushroottable(this->vm_);
      ::sq_pushobject(this->vm_, this->input_);
      if (SQ_FAILED(::sq_call(this->vm_, 2, SQTrue, SQTrue))) {
        throw squirrel_error(this->vm_);
      }
      if (SQ_FAILED(::sq_getstackobj(this->vm_, -1, &this->system_))) {
        throw squirrel_error(this->vm_);
      }
      ::sq_addref(this->vm_, &this->system_);
    }
    {
      /*
       * [Squirrel]
       * game = ::dofile(filename)
       */
      util::stack_restorer r(this->vm_);
      ::sq_pushroottable(this->vm_);
      ::sqstd_dofile(this->vm_,
                     _SC(filename.c_str()),
                     SQTrue,
                     SQTrue);
      if (::sq_gettype(this->vm_, -1) != OT_INSTANCE) {
        throw std::runtime_error("game not found");
      }
      ::sq_getstackobj(this->vm_, -1, &this->game_);
      ::sq_addref(this->vm_, &this->game_);
    }
  }
  virtual
  ~game() {
    ::sq_close(this->vm_);
  }
  bool
  update(ebiten::graphics::texture_factory& tf) {
    if (this->is_terminated_) {
      return true;
    }
    {
      /*
       * [Squirrel]
       * game.update(system_)
       */
      util::stack_restorer r(this->vm_);
      ::sq_pushobject(this->vm_, this->game_);
      ::sq_pushstring(this->vm_, _SC("update"), -1);
      ::sq_get(this->vm_, -2);
      ::sq_pushobject(this->vm_, this->game_);
      ::sq_pushobject(this->vm_, this->system_);
      if (SQ_FAILED(::sq_call(this->vm_, 2, SQFalse, SQTrue))) {
        throw std::runtime_error("Squirrel error happened");
      }
    }
    if (this->is_terminated_) {
      return true;
    }
    texture_class::instantiate(this->vm_, tf);
    return false;
  }
  void
  draw(ebiten::graphics::graphics_context& g,
       ebiten::graphics::texture& main_offscreen) {
    g.set_offscreen(main_offscreen);
    g.clear();
    if (this->is_terminated_) {
      return;
    }
    HSQOBJECT main_offscreen_texture;
    {
      /*
       * [Squirrel]
       * main_offscreen_texture = ebiten.Texture(0, 0)
       */
      util::stack_restorer r(this->vm_);
      ::sq_pushroottable(this->vm_);
      ::sq_pushstring(this->vm_, _SC("ebiten"), -1);
      if (SQ_FAILED(::sq_get(this->vm_, -2))) {
        assert(false);
      }
      ::sq_pushstring(this->vm_, _SC("Texture"), -1);
      if (SQ_FAILED(::sq_get(this->vm_, -2))) {
        assert(false);
      }
      ::sq_pushroottable(this->vm_);
      ::sq_pushinteger(this->vm_, 0);
      ::sq_pushinteger(this->vm_, 0);
      if (SQ_FAILED(::sq_call(this->vm_, 3, SQTrue, SQTrue))) {
        assert(false);
      }
      assert(::sq_gettype(this->vm_, -1) == OT_INSTANCE);
      ::sq_getstackobj(this->vm_, -1, &main_offscreen_texture);
      ::sq_addref(this->vm_, &main_offscreen_texture);
    }
    {
      /*
       * [Squirrel]
       * main_offscreen_texture.setTexture_(main_offscreen)
       */
      util::stack_restorer r(this->vm_);
      ::sq_pushobject(this->vm_, main_offscreen_texture);
      ::sq_pushstring(this->vm_, _SC("setTexture_"), -1);
      ::sq_get(this->vm_, -2);
      ::sq_pushobject(this->vm_, main_offscreen_texture);
      SQUserPointer p = reinterpret_cast<SQUserPointer>(&main_offscreen);
      ::sq_pushuserpointer(this->vm_, p);
      ::sq_call(this->vm_, 2, SQFalse, SQTrue);
    }
    {
      /*
       * [Squirrel]
       * game.draw(main_offscreen_texture)
       */
      util::stack_restorer r(this->vm_);
      ::sq_pushobject(this->vm_, this->game_);
      ::sq_pushstring(this->vm_, _SC("draw"), -1);
      ::sq_get(this->vm_, -2);
      ::sq_pushobject(this->vm_, this->game_);
      ::sq_pushobject(this->vm_, main_offscreen_texture);
      if (SQ_FAILED(::sq_call(this->vm_, 2, SQFalse, SQTrue))) {
        throw std::runtime_error("Squirrel error happened");
      }
    }
    texture_class::flush_texture_commands(this->vm_, g);
    {
      /*
       * [Squirrel]
       * main_offscreen_texture.setTexture_(nullptr)
       */
      util::stack_restorer r(this->vm_);
      ::sq_pushobject(this->vm_, main_offscreen_texture);
      ::sq_pushstring(this->vm_, _SC("setTexture_"), -1);
      ::sq_get(this->vm_, -2);
      ::sq_pushobject(this->vm_, main_offscreen_texture);
      SQUserPointer p = static_cast<SQUserPointer>(nullptr);
      ::sq_pushuserpointer(this->vm_, p);
      ::sq_call(this->vm_, 2, SQFalse, SQTrue);
    }
    {
      /*
       * [Squirrel]
       * (release main_offscreen_texture)
       */
      ::sq_release(this->vm_, &main_offscreen_texture);
    }
  }
  void
  set_terminated_handler(std::function<void()> const& terminated_handler) {
    this->terminated_handler_ = terminated_handler;
  }
  bool
  is_terminated() const {
    return this->is_terminated_;
  }
private:
  static void
  print_func(HSQUIRRELVM, SQChar const* s, ...) {
    va_list vl;
    ::va_start(vl, s);
    ::vfprintf(stdout, s, vl);
    ::va_end(vl);
    ::fflush(stdout);
  }
  static void
  error_func(HSQUIRRELVM, SQChar const* s, ...) {
    va_list vl;
    ::va_start(vl, s);
    ::vfprintf(stderr, s, vl);
    ::va_end(vl);
    ::fflush(stderr);
  }
  void
  initialize_ebiten_classes() {
    static std::string const e("ebiten");
    this->create_table(e);
    {
      util::create_function(this->vm_, e, "terminate", method_terminate, "");
    }
    geometry_matrix_class::initialize(this->vm_);
    input_class::initialize(this->vm_);
    texture_class::initialize(this->vm_);
    system_class::initialize(this->vm_);
  }
  void
  create_table(std::string const& name) {
    util::stack_restorer r(this->vm_);
    ::sq_pushroottable(this->vm_);
    ::sq_pushstring(this->vm_, _SC(name.c_str()), -1);
    ::sq_newtable(this->vm_);
    ::sq_newslot(this->vm_, -3, SQFalse);
  }
  static SQInteger
  method_terminate(HSQUIRRELVM vm) {
    SQUserPointer p = ::sq_getforeignptr(vm);
    game& self = *reinterpret_cast<game*>(p);
    self.is_terminated_ = true;
    if (self.terminated_handler_) {
      self.terminated_handler_();
    }
    return 0;
  }
};

}
}

#endif
