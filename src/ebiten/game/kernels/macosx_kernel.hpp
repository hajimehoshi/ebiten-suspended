#ifndef EBITEN_GAME_KERNELS_MACOSX_KERNEL_HPP
#define EBITEN_GAME_KERNELS_MACOSX_KERNEL_HPP

#include "ebiten/game/graphics/opengl/device.hpp"
#include "ebiten/util/singleton.hpp"

namespace ebiten {
namespace game {
namespace kernels {

class macosx_kernel : public util::singleton<macosx_kernel> {
  friend class util::singleton<macosx_kernel>;
public:
  template<class Game>
  void
  run(Game& game,
      std::size_t screen_width,
      std::size_t screen_height,
      std::size_t fps,
      std::size_t window_scale) {
    auto& device = ebiten::game::graphics::opengl::device::instance();
    device.run(game, screen_width, screen_height, fps, window_scale);
  }
};

}
}
}

#endif
