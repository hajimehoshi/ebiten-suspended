#ifndef EBITEN_SCRIPT_SQUIRREL_GEOMETRY_MATRIX_FUNCS_HPP
#define EBITEN_SCRIPT_SQUIRREL_GEOMETRY_MATRIX_FUNCS_HPP

#include "ebiten/graphics/geometry_matrix.hpp"
#include <squirrel.h> 
#include <sqstdio.h> 
#include <sqstdaux.h>

namespace ebiten_script {
namespace squirrel {

class geometry_matrix_funcs {
private:
  typedef ebiten::graphics::geometry_matrix geometry_matrix;
public:
  static SQInteger constructor(HSQUIRRELVM vm) {
    geometry_matrix* self = new geometry_matrix();
    ::sq_setinstanceup(vm, 1, self);
    ::sq_setreleasehook(vm, 1, releasehook);
    return 0;
  }
  static SQInteger releasehook(SQUserPointer p, SQInteger) {
    geometry_matrix* self = reinterpret_cast<geometry_matrix*>(p);
    delete self;
    return 1;
  }
  static SQInteger meta_tostring(HSQUIRRELVM vm) {
    ::sq_pushstring(vm, _SC("GeometryMatrix!"), -1);
    return 1;
  }
};

}
}

#endif
