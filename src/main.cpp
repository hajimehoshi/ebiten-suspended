#include "ebiten/ebiten.hpp"
#include "sample_game.hpp"

int
main() {
  try {
    sample_game game;
    auto game_update = std::bind(&sample_game::update, &game, std::placeholders::_1);
    auto game_draw   = std::bind(&sample_game::draw,   &game, std::placeholders::_1);
    ebiten::kernels::kernel kernel(game_update,
                                   game_draw,
                                   320, 240, 2, 60);
    kernel.main_loop();
  } catch (std::runtime_error const& err) {
    std::cerr << err.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
