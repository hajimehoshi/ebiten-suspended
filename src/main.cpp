#ifdef EBITEN_TEST
#include <gtest/gtest.h>
#endif

#include "ebiten/ebiten.hpp"
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
    ebiten::game::kernels::kernel kernel;
    kernel.run<sample_game>(320, 240, 600, 2);
  } catch (std::runtime_error const& err) {
    // TODO: use boost::diagnostic_information?
    std::cerr << err.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
