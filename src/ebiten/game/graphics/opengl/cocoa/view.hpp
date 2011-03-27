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

// TODO: private constructor?
template<class Frame>
class view : private boost::noncopyable {
private:
  Frame frame_;
public:
  typedef Frame frame_type;
  view(std::size_t frame_width,
       std::size_t frame_height,
       boost::function<void()> update_device)
    : frame_(frame_width, frame_height) {
    detail::initialize(frame_.native_frame(), frame_.width(), frame_.height(), update_device);
  }
  Frame&
  frame() {
    return this->frame_;
  }
};

}
}
}
}
}

#endif
