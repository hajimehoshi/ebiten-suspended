#define EBITEN_TEST
#include <gtest/gtest.h>

#include "ebiten_script/squirrel/game.hpp"
#include "ebiten/ebiten.hpp"
#include "ebiten/image_loader.hpp"
#include "ebiten/frames/frame.hpp"
#include "ebiten/kernels/application.hpp"
#include <cstdlib>

int
main(int argc, char* argv[]) {
  try {
    std::cout << "Ebiten (Test)" << std::endl
              << "  Commit Date: " << ebiten::version::get_commit_time() << std::endl
              << "  Commit Hash: " << ebiten::version::get_commit_hash() << std::endl;
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
