#ifndef EBITEN_SCRIPT_SQUIRREL_TEXTURE_FUNCS_HPP
#define EBITEN_SCRIPT_SQUIRREL_TEXTURE_FUNCS_HPP

#include <squirrel.h> 
#include <sqstdio.h> 
#include <sqstdaux.h>

namespace ebiten_script {
namespace squirrel {

class texture_funcs {
public:
  static SQInteger constructor(HSQUIRRELVM) {
    /*SQInteger n_args = ::sq_gettop(vm);
    if (n_args < 1) {
      return ::sq_throwerror(vm, _SC("too few arguments"));
    }
    if (2 < n_args) {
      return ::sq_throwerror(vm, _SC("too many arguments"));
    }
    if (n_args == 1) {
      if (::sq_gettype(vm, 1) != OT_STRING) {
        return ::sq_throwerror(vm, _SC("invalid argument"));
      }
      SQChar const** path;
      ::sq_getstring(vm, 1, path);
      //new ebiten::graphics::
    }
    if (n_args == 2) {
      
    }*/
    return 0;
  }
};

}
}

#endif
