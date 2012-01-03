#ifndef EBITEN_SCRIPT_SQUIRREL_SPRITE_CLASS_HPP
#define EBITEN_SCRIPT_SQUIRREL_SPRITE_CLASS_HPP

#include "ebiten_script/squirrel/squirrel_error.hpp"
//#include "ebiten_script/squirrel/texture_class.hpp"
#include "ebiten_script/sprite.hpp"

namespace ebiten_script {
namespace squirrel {

class sprite_class {
public:
  sprite_class() = delete;
  sprite_class(sprite_class const&) = delete;
  sprite_class& operator=(sprite_class const&) = delete;
  static SQUserPointer
  type_tag() {
    return reinterpret_cast<SQUserPointer>(method_constructor);
  }
  static SQInteger
  method_constructor(HSQUIRRELVM vm) {
    /*SQUserPointer p;
    if (SQ_FAILED(::sq_getinstanceup(vm, 2, &p, texture_class::type_tag()))) {
      return ::sq_throwerror(vm, "failed to get an instance");
      }*/
    /*sprite* self = new sprite(texture);
    ::sq_setinstanceup(vm, 1, reinterpret_cast<self>);
    ::sq_setreleasehook(vm, 1, releasehook);*/
    return 0;
  }
  static SQInteger
  releasehook(SQUserPointer p, SQInteger) {
    class sprite* self = reinterpret_cast<sprite*>(p);
    delete self;
    return 1;
  }
  static sprite&
  get_self(HSQUIRRELVM vm, SQInteger idx) {
    SQUserPointer p;
    {
      SQRESULT result = ::sq_getinstanceup(vm, idx, &p, type_tag());
      if (SQ_FAILED(result)) {
        throw squirrel_error(result);
      }
    }
    sprite* self = reinterpret_cast<sprite*>(p);
    return *self;
  }
};

}
}

#endif
