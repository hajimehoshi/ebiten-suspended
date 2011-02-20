#ifdef EBITEN_TEST
#include <gtest/gtest.h>
#endif

#include "ebiten/game/kernels/macosx/kernel.hpp"
#include "sample_game.hpp"

int
main(int argc, char** argv) {
  (void)argc;
  (void)argv;
#ifdef EBITEN_TEST
  testing::InitGoogleTest(&argc, argv);
  int const test_result = RUN_ALL_TESTS();
  if (test_result != EXIT_SUCCESS) {
    return test_result;
  }
  if (2 <= argc) {
    return EXIT_SUCCESS;
  }
#endif
  try {
    typedef ebiten::game::kernels::macosx::kernel kernel_type;
    kernel_type& kernel = kernel_type::instance();
    sample_game game;
    kernel.run(game, 320, 240, 600, 2);
  } catch (std::string const& message) {
    // TODO: use boost::diagnostic_information?
    std::cerr << message << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
