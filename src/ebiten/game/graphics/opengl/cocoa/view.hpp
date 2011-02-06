#ifndef EBITEN_GAME_GRAPHICS_OPENGL_COCOA_HPP
#define EBITEN_GAME_GRAPHICS_OPENGL_COCOA_HPP

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <cstddef>

namespace ebiten {
namespace game {
namespace graphics {
namespace opengl {
namespace cocoa {

namespace detail {

void initialize(std::ptrdiff_t native_frame);

}

template<class Frame, class Device>
class view : private boost::noncopyable {
public:
  explicit
  view(Frame& frame, Device& device)
    : device_(device) {
    detail::initialize(frame.native_frame());
  }
private:
  Device& device_;
};

}
}
}
}
}

#endif
