#ifndef EBITEN_GRAPHICS_DRAWING_REGION_HPP
#define EBITEN_GRAPHICS_DRAWING_REGION_HPP

namespace ebiten {
namespace graphics {

class drawing_region {

private:
  // TODO: implements by std::unique_ptr<std::array<double, 6> >?
  double src_x_, src_y_, dst_x_, dst_y_, width_, height_;
public:
  drawing_region()
    : src_x_(0), src_y_(0), dst_x_(0), dst_y_(0),
      width_(0), height_(0) {
  }
  drawing_region(double src_x,
                 double src_y,
                 double dst_x,
                 double dst_y,
                 double width,
                 double height)
    : src_x_(src_x), src_y_(src_y), dst_x_(dst_x), dst_y_(dst_y),
      width_(width), height_(height) {
  }
  double src_x() const {
    return this->src_x_;
  }
  double src_y() const {
    return this->src_y_;
  }
  double dst_x() const {
    return this->dst_x_;
  }
  double dst_y() const {
    return this->dst_y_;
  }
  double width() const {
    return this->width_;
  }
  double height() const {
    return this->height_;
  }
  void set_src_x(double value) {
    this->src_x_ = value;
  }
  void set_src_y(double value) {
    this->src_y_ = value;
  }
  void set_dst_x(double value) {
    this->dst_x_ = value;
  }
  void set_dst_y(double value) {
    this->dst_y_ = value;
  }
  void set_width(double value) {
    this->width_ = value;
  }
  void set_height(double value) {
    this->height_ = value;
  }
};

}
}

#endif
