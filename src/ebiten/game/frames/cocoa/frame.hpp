#ifndef EBITEN_GAME_FRAMES_FRAME_HPP
#define EBITEN_GAME_FRAMES_FRAME_HPP

#include <boost/noncopyable.hpp>

namespace ebiten {
namespace game {
namespace frames {
namespace cocoa {

class frame : private boost::noncopyable {
public:
  int run();
};

}
}
}
}

#endif
