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

class view : private boost::noncopyable {
public:
  explicit view(const std::ptrdiff_t native_frame);
  ~view();
private:
  struct impl;
  boost::scoped_ptr<impl> pimpl_;
};

}
}
}
}
}

#endif
