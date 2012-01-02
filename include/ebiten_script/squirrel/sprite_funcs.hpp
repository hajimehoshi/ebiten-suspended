#ifndef EBITEN_SCRIPT_SQUIRREL_SPRITE_FUNCS_HPP
#define EBITEN_SCRIPT_SQUIRREL_SPRITE_FUNCS_HPP

namespace ebiten_script {
namespace squirrel {

class sprite_funcs {
public:
  sprite_funcs() = delete;
  sprite_funcs(sprite_funcs const&) = delete;
  sprite_funcs& operator=(sprite_funcs const&) = delete;
  static SQInteger
  method_constructor(HSQUIRRELVM vm) {
    
    return 0;
  }
}

}
}

#endif
