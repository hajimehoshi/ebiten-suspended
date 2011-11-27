#include "ebiten/ebiten.hpp"
#include "sample_game.hpp"

int
main() {
  try {
    ebiten::kernels::kernel kernel;
    kernel.run<sample_game>(320, 240, 600, 2);
  } catch (std::runtime_error const& err) {
    // TODO: use boost::diagnostic_information?
    std::cerr << err.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
