#ifndef EBITEN_GRAPHICS_DRAWING_REGION_HPP
#define EBITEN_GRAPHICS_DRAWING_REGION_HPP

namespace ebiten {
namespace graphics {

class drawing_region {
public:
  double src_x, src_y, dst_x, dst_y, width, height;
  drawing_region()
    : src_x(0),
      src_y(0),
      dst_x(0),
      dst_y(0),
      width(0),
      height(0) {
  }
  drawing_region(double src_x_,
                 double src_y_,
                 double dst_x_,
                 double dst_y_,
                 double width_,
                 double height_)
    : src_x(src_x_),
      src_y(src_y_),
      dst_x(dst_x_),
      dst_y(dst_y_),
      width(width_),
      height(height_) {
  }
};

}
}

#endif
