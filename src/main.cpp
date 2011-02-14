#ifdef EBITEN_TEST
#include <gtest/gtest.h>
#endif

#include "ebiten/game/kernels/macosx/kernel.hpp"
#include "sample_game.hpp"
#include <boost/typeof/typeof.hpp>

int
main(int argc, char** argv) {
  (void)argc;
  (void)argv;
#ifdef EBITEN_TEST
  testing::InitGoogleTest(&argc, argv);
  const int test_result = RUN_ALL_TESTS();
  if (test_result != EXIT_SUCCESS) {
    return test_result;
  }
  if (2 <= argc) {
    return EXIT_SUCCESS;
  }
#endif
  try {
    sample_game game;
    ebiten::game::kernels::macosx::kernel::instance().run(game, 320, 240, 600, 2);
  } catch (const std::string& message) {
    // TODO: use boost::diagnostic_information?
    std::cerr << message << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
