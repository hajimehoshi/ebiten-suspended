#ifndef EBITEN_GAME_GRAPHICS_OPENGL_COCOA_HPP
#define EBITEN_GAME_GRAPHICS_OPENGL_COCOA_HPP

#include <boost/noncopyable.hpp>
#include <cstddef>

namespace ebiten {
namespace game {
namespace graphics {
namespace opengl {
namespace cocoa {

class view : private boost::noncopyable {
public:
  explicit view(const std::ptrdiff_t native_frame);
private:
  void* gl_view_;
};

}
}
}
}
}

#endif
