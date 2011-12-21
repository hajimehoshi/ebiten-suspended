#include "samples/sample_game.hpp"
#include "ebiten/ebiten.hpp"
#include "ebiten/frames/frame.hpp"
#include "ebiten/kernels/application.hpp"

int
main() {
  try {
    samples::sample_game game;
    auto game_update = std::bind(&samples::sample_game::update,
                                 &game,
                                 std::placeholders::_1);
    auto game_draw   = std::bind(&samples::sample_game::draw,
                                 &game,
                                 std::placeholders::_1);
    ebiten::frames::frame frame(640, 480);
    ebiten::kernels::kernel kernel(game_update,
                                   game_draw,
                                   320, 240, 2, 60,
                                   frame.native_view());
    ebiten::kernels::application::run(frame.native_frame());
  } catch (std::runtime_error const& err) {
    std::cerr << err.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

