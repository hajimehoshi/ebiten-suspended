#include "ebiten_application/run.hpp"
#include "ebiten_frame/frame.hpp"
#include "ebiten_script/v8/game.hpp"
#include "ebiten/ebiten.hpp"
#include "ebiten/resources.hpp"

int
main() {
  try {
    std::cout << "Ebiten" << std::endl
              << "  Commit Date: " << ebiten::version::get_commit_time() << std::endl
              << "  Commit Hash: " << ebiten::version::get_commit_hash() << std::endl
              << "  Commit Modified: " << (ebiten::version::is_commit_modified() ? "Yes" : "No") << std::endl;

    std::string script_path = ebiten::get_resource_path("sprites.js");
    ebiten_script::v8::game game(script_path);
    auto game_update = std::bind(&ebiten_script::v8::game::update,
                                 &game,
                                 std::placeholders::_1,
                                 std::placeholders::_2);
    auto game_draw = std::bind(&ebiten_script::v8::game::draw,
                               &game,
                               std::placeholders::_1,
                               std::placeholders::_2);
    ebiten_frame::frame frame(640, 480);
    game.set_terminated_handler(std::bind(&ebiten_frame::frame::close, &frame));
    ebiten::kernel kernel(game_update,
                          game_draw,
                          320, 240, 2, 60,
                          frame.native_view());
    ebiten_application::run(frame.native_frame());
  } catch (std::runtime_error const& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
