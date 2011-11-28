#include "ebiten/ebiten.hpp"
#include "sample_game.hpp"

int
main() {
  try {
    sample_game game;
    ebiten::kernel::run(game, 320, 240, 600, 2);
  } catch (std::runtime_error const& err) {
    std::cerr << err.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
