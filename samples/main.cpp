#include "sample_game.hpp"
#include "ebiten/ebiten.hpp"
#include "ebiten/frames/frame.hpp"
#include "ebiten/kernels/application.hpp"

#include <squirrel.h> 
#include <sqstdio.h> 
#include <sqstdaux.h>

static void
printfunc(HSQUIRRELVM, SQChar const* s, ...) {
  va_list vl;
  ::va_start(vl, s);
  ::vfprintf(stdout, s, vl);
  ::va_end(vl);
}

static void
errorfunc(HSQUIRRELVM, SQChar const* s, ...) {
  va_list vl;
  ::va_start(vl, s);
  ::vfprintf(stderr, s, vl);
  ::va_end(vl);
}

int
main() {
  {
    HSQUIRRELVM v = ::sq_open(1024);
    ::sqstd_seterrorhandlers(v);
    ::sq_setprintfunc(v, ::printfunc, ::errorfunc);
    ::sq_pushroottable(v);
    NSBundle* bundle = [NSBundle mainBundle];
    NSString* ns_path = [bundle pathForResource:@"test.nut" ofType:nil];
    std::string path;
    if (ns_path != nil) {
      path = [ns_path UTF8String];
    }
    if (SQ_SUCCEEDED(::sqstd_dofile(v, _SC(path.c_str()), SQFalse, SQTrue))) {
      // TODO: Main game loop is here
    }
    ::sq_pop(v, 1);
    ::sq_close(v);
  }
  try {
    sample_game game;
    auto game_update = std::bind(&sample_game::update,
                                 &game,
                                 std::placeholders::_1);
    auto game_draw   = std::bind(&sample_game::draw,
                                 &game,
                                 std::placeholders::_1);
    ebiten::frames::frame frame(640, 480);
    ebiten::kernels::kernel kernel(game_update,
                                   game_draw,
                                   320, 240, 2, 60,
                                   frame.native_view());
                                   ebiten::kernels::application::run(frame.native_frame());
  } catch (std::runtime_error const& err) {
    std::cerr << err.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
