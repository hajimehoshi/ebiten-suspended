#define EBITEN_TEST
#include <gtest/gtest.h>

#include "ebiten_script/squirrel/game.hpp"
#include "ebiten_frame/frame.hpp"
#include "ebiten/ebiten.hpp"
#include "ebiten/image_loader.hpp"
#include "ebiten_application/run.hpp"
#include <cstdlib>

int
main(int argc, char* argv[]) {
  try {
    std::cout << "Ebiten (Test)" << std::endl
              << "  Commit Date: " << ebiten::version::get_commit_time() << std::endl
              << "  Commit Hash: " << ebiten::version::get_commit_hash() << std::endl
              << "  Commit Modified: " << (ebiten::version::is_commit_modified() ? "Yes" : "No") << std::endl;
    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    if (result != EXIT_SUCCESS) {
      return result;
    }
    ebiten_script::squirrel::game game("./test/test.nut");
    auto game_update = std::bind(&ebiten_script::squirrel::game::update,
                                 &game,
                                 std::placeholders::_1);
    auto game_draw = std::bind(&ebiten_script::squirrel::game::draw,
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
