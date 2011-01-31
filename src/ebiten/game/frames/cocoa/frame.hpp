#ifndef EBITEN_GAME_FRAMES_FRAME_HPP
#define EBITEN_GAME_FRAMES_FRAME_HPP

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <cstddef>

namespace ebiten {
namespace game {
namespace frames {
namespace cocoa {

class frame : private boost::noncopyable {
public:
  frame(std::size_t width, std::size_t height);
  ~frame();
  std::ptrdiff_t native_frame() const;
  int run();
private:
  struct impl;
  boost::scoped_ptr<impl> pimpl_;
};

}
}
}
}

#endif
