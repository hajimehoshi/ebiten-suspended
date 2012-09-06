#include "game.hpp"
#include "ebiten_application/run.hpp"
#include "ebiten_frame/frame.hpp"
#include "ebiten/ebiten.hpp"

int
main() {
  try {
    std::cout << "Ebiten" << std::endl
              << "  Commit Date: " << ebiten::version::get_commit_time() << std::endl
              << "  Commit Hash: " << ebiten::version::get_commit_hash() << std::endl
              << "  Commit Modified: " << (ebiten::version::is_commit_modified() ? "Yes" : "No") << std::endl;
    game g;
    ebiten_frame::frame::frame frame(640, 480);
    ebiten::kernel kernel([&](ebiten::graphics::texture_factory& tf, ebiten::input const& input) {
        return g.update(tf, input);
      },
      [&](ebiten::graphics::graphics_context& gr, ebiten::graphics::texture& t) {
        return g.draw(gr, t);
      },
      320, 240, 2, 60,
      frame.native_view());
    ebiten_application::run(frame.native_frame());
  } catch (std::runtime_error const& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
