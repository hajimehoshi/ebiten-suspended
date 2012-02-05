#ifndef EBITEN_SCRIPT_SQUIRREL_GAME_HPP
#define EBITEN_SCRIPT_SQUIRREL_GAME_HPP

#include "ebiten_script/squirrel/color_matrix_class.hpp"
#include "ebiten_script/squirrel/geometry_matrix_class.hpp"
#include "ebiten_script/squirrel/input_class.hpp"
#include "ebiten_script/squirrel/texture_class.hpp"
#include "ebiten_script/squirrel/system_class.hpp"
#include "ebiten/graphics/graphics_context.hpp"
#include "ebiten/graphics/texture_factory.hpp"
#include "ebiten/input.hpp"
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
  HSQOBJECT ebiten_;
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
    this->input_ = util::call(this->vm_, this->ebiten_, "Input", true);
    this->system_ = util::call(this->vm_, this->ebiten_, "System", true, this->input_);
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
  update(ebiten::graphics::texture_factory& tf,
         ebiten::input const& input) {
    if (this->is_terminated_) {
      return true;
    }
    util::call(this->vm_, this->input_, "setInput_", false,
               reinterpret_cast<SQUserPointer>(const_cast<ebiten::input*>(&input)));
    util::call(this->vm_, this->game_, "update", false,
               this->system_);
    util::call(this->vm_, this->input_, "setInput_", false,
               static_cast<SQUserPointer>(nullptr));
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
    // TODO: check references of local variables
    HSQOBJECT main_offscreen_texture =
      util::call(this->vm_, this->ebiten_, "Texture", true,
                 0, 0);
    util::call(this->vm_, main_offscreen_texture, "setTexture_", false,
               reinterpret_cast<SQUserPointer>(&main_offscreen));
    util::call(this->vm_, this->game_, "draw", false,
               main_offscreen_texture);
    texture_class::flush_texture_commands(this->vm_, g);
    util::call(this->vm_, main_offscreen_texture, "setTexture_", false,
               static_cast<SQUserPointer>(nullptr));
    ::sq_release(this->vm_, &main_offscreen_texture);
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
    {
      // TODO: ebiten should be a class?
      /*
       * [Squirrel]
       * ebiten = {}
       */
      util::stack_restorer r(this->vm_);
      ::sq_pushroottable(this->vm_);
      ::sq_pushstring(this->vm_, _SC(e.c_str()), -1);
      ::sq_newtable(this->vm_);
      ::sq_getstackobj(this->vm_, -1, &this->ebiten_);
      ::sq_newslot(this->vm_, -3, SQFalse);
    }
    {
      util::create_function(this->vm_, e, "terminate", method_terminate, "");
    }
    color_matrix_class::initialize(this->vm_);
    geometry_matrix_class::initialize(this->vm_);
    input_class::initialize(this->vm_);
    texture_class::initialize(this->vm_);
    system_class::initialize(this->vm_);
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
