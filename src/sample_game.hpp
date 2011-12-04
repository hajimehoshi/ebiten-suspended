#ifndef SAMPLE_GAME_HPP
#define SAMPLE_GAME_HPP

#include "ebiten/ebiten.hpp"
#include <cstdlib>
#include <iostream>

class sprite {
public:
  ebiten::graphics::drawing_region drawing_region;
};

class sample_game : private ebiten::noncopyable {
private:
  typedef std::vector<sprite> sprites_type;
  ebiten::graphics::texture_id texture_id_;
  std::size_t texture_width_;
  std::size_t texture_height_;
  sprites_type sprites_;
public:
  sample_game() {
  }
  void
  initialize(ebiten::graphics::device::texture_factory_type& tf) {
    NSBundle* bundle = [NSBundle mainBundle];
    NSString* path = [bundle pathForResource:@"test.png" ofType:nil];
    std::string path2([path UTF8String]);
    auto t = tf.from_file(path2);
    this->texture_id_     = t.id();
    this->texture_width_  = t.width();
    this->texture_height_ = t.height();
    typedef ebiten::graphics::drawing_region dr;
    this->sprites_.emplace_back();
    this->sprites_.emplace_back();
    this->sprites_.emplace_back();
    this->sprites_.emplace_back();
    this->sprites_.emplace_back();
    this->sprites_.emplace_back();
    this->sprites_.at(0).drawing_region = dr(0, 0, 32, 32, 32, 32);
    this->sprites_.at(1).drawing_region = dr(0, 0, 32, 132, 32, 32);
    this->sprites_.at(2).drawing_region = dr(0, 0, 132, 32, 32, 32);
    this->sprites_.at(3).drawing_region = dr(0, 0, 132, 132, 32, 32);
    this->sprites_.at(4).drawing_region = dr(0, 0, 32, 32, 32, 32);
    this->sprites_.at(5).drawing_region = dr(0, 0, 64, 32, 32, 32);
  }
  void
  update(int frame_count) {
    // update 中に draw が実行されても困る
    // draw は描画時だけ実行されれば良い
    auto& s = this->sprites_.at(0);
    auto& dr = s.drawing_region;
    dr.dst_x = 32 + 0.1 * (frame_count % 2400);
    if (frame_count % 600 == 0) {
      std::cout << frame_count << std::endl;
    }
  }
  void
  draw(ebiten::graphics::device::graphics_context_type& gc) const {
    gc.set_texture(this->texture_id_,
                   this->texture_width_,
                   this->texture_height_);
    /*auto mat = ebiten::graphics::geometry_matrix(1, 0, 0, 1, 32, 32);
      gc.set_geometry_matrix(mat);*/
    for (auto const& s : this->sprites_) {
      //gc.set_color_matrix(s.color_matrix);
      gc.draw(s.drawing_region);
    }
  }
};

#endif
