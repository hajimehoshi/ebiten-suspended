#ifndef EBITEN_GAME_KERNELS_MACOSX_KERNEL_HPP
#define EBITEN_GAME_KERNELS_MACOSX_KERNEL_HPP

#include "ebiten/game/frames/cocoa/frame.hpp"
#include "ebiten/game/graphics/opengl/device.hpp"
#include "ebiten/game/timers/mach/timer.hpp"
#include "ebiten/util/singleton.hpp"

namespace ebiten {
namespace game {
namespace kernels {
namespace macosx {

class kernel : public util::singleton<kernel> {
  friend class util::singleton<kernel>;
public:
  template<class Game>
  void
  /*run(Game& game,
      std::size_t screen_width,
      std::size_t screen_height,
      std::size_t fps,
      std::size_t window_scale) {*/
  run(Game&,
      std::size_t screen_width,
      std::size_t screen_height,
      std::size_t,
      std::size_t window_scale) {
    /*auto& device = ebiten::game::graphics::opengl::device::instance();
    timers::mach::timer timer(fps);
    device.run(game, screen_width, screen_height, window_scale, timer);*/
    ebiten::game::frames::cocoa::frame frame(screen_width * window_scale, screen_height * window_scale);
    frame.run();
  }
};

}
}
}
}

#endif
