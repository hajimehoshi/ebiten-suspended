#include "ebiten/ebiten.hpp"
#include "sample_game.hpp"

int
main() {
  try {
    sample_game game;
    ebiten::kernels::kernel kernel;
    kernel.run(game, 320, 240, 60, 2);
  } catch (std::runtime_error const& err) {
    std::cerr << err.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
