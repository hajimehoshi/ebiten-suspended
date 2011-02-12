#ifndef EBITEN_GAME_KERNELS_MACOSX_KERNEL_HPP
#define EBITEN_GAME_KERNELS_MACOSX_KERNEL_HPP

#include "ebiten/game/frames/cocoa/frame.hpp"
#include "ebiten/game/graphics/opengl/device.hpp"
#include "ebiten/game/graphics/opengl/cocoa/view.hpp"
#include "ebiten/game/kernels/macosx/application.hpp"
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
  run(Game& game,
      std::size_t screen_width,
      std::size_t screen_height,
      std::size_t /* fps */,
      std::size_t window_scale) {
    //timers::mach::timer timer(fps);
    frames::cocoa::frame frame(screen_width * window_scale, screen_height * window_scale);
    typedef decltype(game.sprites()) sprites_cref;
    std::function<sprites_cref()> get_sprites = [&]()->sprites_cref {
      return game.sprites();
    };
    int frame_count = 0;
    std::function<void()> update_game = [&]{
      game.update(frame_count);
      ++frame_count;
    };
    graphics::opengl::device<sprites_cref> device(screen_width, screen_height, window_scale,
                                                  get_sprites, update_game);
    graphics::opengl::cocoa::view<decltype(frame)> view(frame,
                                                        [&]{
                                                          device.update();
                                                        });
    // TODO: remove initialize...
    device.initialize();
    game.initialize(graphics::opengl::texture_factory::instance());
    application::instance().run(frame);
  }
};

}
}
}
}

#endif
