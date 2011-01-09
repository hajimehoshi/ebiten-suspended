#ifndef EBITEN_GAME_VIDEO_DRAWING_REGION_HPP
#define EBITEN_GAME_VIDEO_DRAWING_REGION_HPP

#include <boost/noncopyable.hpp>
#include <algorithm>

namespace ebiten {
namespace game {
namespace video {

class drawing_region : private boost::noncopyable {
public:
  // TODO: implements by std::array?
  double src_x, src_y, dst_x, dst_y, width, height;
  drawing_region(double src_x_,
                 double src_y_,
                 double dst_x_,
                 double dst_y_,
                 double width_,
                 double height_)
    : src_x(src_x_), src_y(src_y_), dst_x(dst_x_), dst_y(dst_y_),
      width(width_), height(height_) {
  }
  drawing_region(drawing_region&& rhs)
    : src_x(rhs.src_x), src_y(rhs.src_y), dst_x(rhs.dst_x), dst_y(rhs.dst_y),
      width(rhs.width), height(rhs.height) {
  }
  drawing_region& operator=(drawing_region&& rhs) {
    this->src_x = rhs.src_x;
    this->src_y = rhs.src_y;
    this->dst_x = rhs.dst_x;
    this->dst_y = rhs.dst_y;
    this->width = rhs.width;
    this->height = rhs.height;
    return *this;
  }
  /*void
  swap(drawing_region& rhs) throw() {
    std::swap(this->src_x, rhs.src_x);
    std::swap(this->src_y, rhs.src_y);
    std::swap(this->dst_x, rhs.dst_x);
    std::swap(this->dst_y, rhs.dst_y);
    std::swap(this->width, rhs.width);
    std::swap(this->height, rhs.height);
    }*/
};

}
}
}

/*namespace std {

template<>
void
swap(ebiten::game::video::drawing_region& dr1,
     ebiten::game::video::drawing_region& dr2) throw() {
  dr1.swap(dr2);
}

}*/

#endif
