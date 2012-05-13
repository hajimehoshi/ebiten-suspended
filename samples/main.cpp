#include "ebiten_application/run.hpp"
#include "ebiten_frame/frame.hpp"
#include "ebiten/ebiten.hpp"

class game : private ebiten::noncopyable {
public:
  bool
  update(ebiten::graphics::texture_factory&,
         ebiten::input const&) {
    return false;
  }
  void
  draw(ebiten::graphics::graphics_context& g,
       ebiten::graphics::texture& offscreen) {
    g.set_offscreen(offscreen);
    g.clear();
  }
};

int
main() {
  try {
    std::cout << "Ebiten" << std::endl
              << "  Commit Date: " << ebiten::version::get_commit_time() << std::endl
              << "  Commit Hash: " << ebiten::version::get_commit_hash() << std::endl
              << "  Commit Modified: " << (ebiten::version::is_commit_modified() ? "Yes" : "No") << std::endl;
    game g;
    auto game_update = std::bind(&game::update,
                                 &g,
                                 std::placeholders::_1,
                                 std::placeholders::_2);
    auto game_draw = std::bind(&game::draw,
                               &g,
                               std::placeholders::_1,
                               std::placeholders::_2);
    ebiten_frame::frame::frame frame(640, 480);
    ebiten::kernel kernel(game_update,
                          game_draw,
                          320, 240, 2, 60,
                          frame.native_view());
    ebiten_application::run(frame.native_frame());
  } catch (std::runtime_error const& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
