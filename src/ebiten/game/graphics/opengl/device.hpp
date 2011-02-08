#ifndef EBITEN_GAME_GRAPHICS_OPENGL_DEVICE_HPP
#define EBITEN_GAME_GRAPHICS_OPENGL_DEVICE_HPP

#include "ebiten/game/graphics/opengl/graphics_context.hpp"
#include "ebiten/game/graphics/opengl/texture_factory.hpp"
#include "ebiten/game/graphics/sprite.hpp"
#include "ebiten/util/singleton.hpp"
#include <OpenGL/gl.h>
#include <boost/range.hpp>
#include <atomic>
#include <cassert>
#include <pthread.h>

namespace ebiten {
namespace game {
namespace graphics {
namespace opengl {

class device : public util::singleton<device> {
  friend class util::singleton<device>;
public:
  void
  update() {
  }
private:
  device() {
  }
  ~device() {
  }
};

}
}
}
}

#endif
