#define EBITEN_TEST
#include <gtest/gtest.h>

#include "ebiten_application/run.hpp"
#include "ebiten_frame/frame.hpp"
#include "ebiten/ebiten.hpp"
#include "ebiten/image_loader.hpp"
#include "ebiten/resources.hpp"
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
  } catch (std::runtime_error const& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
