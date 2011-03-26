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

void initialize(util::id_ const& native_frame,
                std::size_t width,
                std::size_t height,
                boost::function<void()>& update_device);

}

// TODO: singleton?
template<class Frame>
class view : private boost::noncopyable {
public:
  typedef Frame frame_type;
  view(Frame& frame,
       boost::function<void()> update_device) {
    detail::initialize(frame.native_frame(), frame.width(), frame.height(), update_device);
  }
};

}
}
}
}
}

#endif
