#ifndef EBITEN_GAME_GRAPHICS_OPENGL_COCOA_HPP
#define EBITEN_GAME_GRAPHICS_OPENGL_COCOA_HPP

#include "ebiten/util/id.hpp"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/type_traits.hpp>

namespace ebiten {
namespace game {
namespace graphics {
namespace opengl {
namespace cocoa {

namespace detail {

void initialize(const util::id_& native_frame, boost::function<void()> update_device);

}

template<class Frame>
class view : private boost::noncopyable {
public:
  view(Frame& frame, boost::function<void()> update_device) {
    detail::initialize(frame.native_frame(), update_device);
  }
};

}
}
}
}
}

#endif
