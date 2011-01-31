#ifndef EBITEN_GAME_FRAMES_FRAME_HPP
#define EBITEN_GAME_FRAMES_FRAME_HPP

#include <boost/noncopyable.hpp>
#include <cstddef>

namespace ebiten {
namespace game {
namespace frames {
namespace cocoa {

class frame : private boost::noncopyable {
public:
  frame(std::size_t width, std::size_t height);
  std::ptrdiff_t native_frame() const;
  int run();
private:
  void* window_;
};

}
}
}
}

#endif
