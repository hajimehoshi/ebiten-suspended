#ifndef nullptr
#define nullptr (0)
#endif

#ifdef EBITEN_TEST
#include <gtest/gtest.h>
#endif

#include "ebiten/game/kernels/macosx_kernel.hpp"
#include "sample_game.hpp"

int
main(int argc, char** argv) {
  (void)argc;
  (void)argv;
#ifdef EBITEN_TEST
  testing::InitGoogleTest(&argc, argv);
  const auto test_result = RUN_ALL_TESTS();
  if (test_result != EXIT_SUCCESS) {
    return test_result;
  }
  if (2 <= argc) {
    return EXIT_SUCCESS;
  }
#endif
  try {
    sample_game game;
    auto& kernel = ebiten::game::kernels::macosx_kernel::instance();
    kernel.run(game, 320, 240, 600, 2);
  } catch (const std::string& message) {
    // TODO: use boost::diagnostic_information?
    std::cerr << message << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
