#include "ebiten_script/squirrel/game.hpp"
#include "ebiten/ebiten.hpp"
#include "ebiten/frames/frame.hpp"
#include "ebiten/kernels/application.hpp"

int
main() {
  try {
    std::cout << "Ebiten" << std::endl
              << "  Commit Date: " << ebiten::version::get_commit_time() << std::endl
              << "  Commit Hash: " << ebiten::version::get_commit_hash() << std::endl;

    NSBundle* bundle = [NSBundle mainBundle];
    NSString* ns_path = [bundle pathForResource:@"test.nut" ofType:nil];
    std::string path;
    if (ns_path != nil) {
      path = [ns_path UTF8String];
    } else {
      path = "test.nut";
    }
    ebiten_script::squirrel::game game(path);
    auto game_update = std::bind(&ebiten_script::squirrel::game::update,
                                 &game,
                                 std::placeholders::_1);
    auto game_draw = std::bind(&ebiten_script::squirrel::game::draw,
                               &game,
                               std::placeholders::_1,
                               std::placeholders::_2);
    auto game_is_terminated = std::bind(&ebiten_script::squirrel::game::is_terminated,
                                        &game);
    ebiten::frames::frame frame(640, 480, game_is_terminated);
    ebiten::kernels::kernel kernel(game_update,
                                   game_draw,
                                   320, 240, 2, 60,
                                   frame.native_view());
    ebiten::kernels::application::run(frame.native_frame());
  } catch (std::runtime_error const& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
