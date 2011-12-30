#ifndef EBITEN_SCRIPT_SQUIRREL_TEXTURE_FACTORY_FUNCS_HPP
#define EBITEN_SCRIPT_SQUIRREL_TEXTURE_FACTORY_FUNCS_HPP

#include "ebiten/graphics/texture_factory.hpp"
#include <squirrel.h> 
#include <sqstdio.h> 
#include <sqstdaux.h>

namespace ebiten_script {
namespace squirrel {

class texture_factory_funcs {
public:
  static SQInteger
  constructor(HSQUIRRELVM vm) {
    /*SQInteger const n_args = ::sq_gettop(vm);
    if (n_args != 2) {
      return ::sq_throwerror(vm, _SC("invalid number of arguments"));
    }
    if (::sq_gettype(vm, 2) != OT_USERPOINTER) {
      return ::sq_throwerror(vm, _SC("invalid type"));
      }*/
    SQUserPointer user_pointer;
    ::sq_getuserpointer(vm, 2, &user_pointer);
    ebiten::graphics::texture_factory* self =
      reinterpret_cast<ebiten::graphics::texture_factory*>(user_pointer);
    ::sq_setinstanceup(vm, 2, self);
    return 0;
  }
  static SQInteger
  meta_typeof(HSQUIRRELVM vm) {
    ::sq_pushstring(vm, _SC("ebiten.TextureFactory"), -1);
    return 1;
  }
  static SQInteger
  load(HSQUIRRELVM) {
    std::cout << "load" << std::endl;
    //SQInteger n_args = sq_gettop(vm);
    return 0; // 1
  }
  static SQInteger
  create(HSQUIRRELVM) {
    return 0; // 1
  }
};

}
}

#endif
