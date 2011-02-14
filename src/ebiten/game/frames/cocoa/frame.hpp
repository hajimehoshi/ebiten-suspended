#ifndef EBITEN_GAME_FRAMES_FRAME_HPP
#define EBITEN_GAME_FRAMES_FRAME_HPP

#include "ebiten/util/id.hpp"
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <cstddef>

namespace ebiten {
namespace game {
namespace frames {
namespace cocoa {

class frame : private boost::noncopyable {
private:
  std::size_t const width_;
  std::size_t const height_;
  util::id_ window_;
public:
  frame(std::size_t width, std::size_t height);
  ~frame();
  util::id_ const& native_frame() const;
  std::size_t width() const {
    return this->width_;
  }
  std::size_t height() const {
    return this->height_;
  }
};

}
}
}
}

#endif
