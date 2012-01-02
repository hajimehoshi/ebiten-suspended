#ifndef EBITEN_SCRIPT_SQUIRREL_SPRITE_CLASS_HPP
#define EBITEN_SCRIPT_SQUIRREL_SPRITE_CLASS_HPP

namespace ebiten_script {
namespace squirrel {

class sprite_class {
public:
  sprite_class() = delete;
  sprite_class(sprite_class const&) = delete;
  sprite_class& operator=(sprite_class const&) = delete;
  static SQInteger
  method_constructor(HSQUIRRELVM vm) {
    
    return 0;
  }
}

}
}

#endif
