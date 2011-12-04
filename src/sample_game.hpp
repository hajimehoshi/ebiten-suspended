#ifndef SAMPLE_GAME_HPP
#define SAMPLE_GAME_HPP

#include "ebiten/ebiten.hpp"
#include <cstdlib>
#include <deque>
#include <iostream>

class sprite {
public:
  ebiten::graphics::drawing_region drawing_region;
};

class sample_game : private ebiten::noncopyable {
private:
  typedef std::vector<sprite> sprites_type;
  std::unique_ptr<ebiten::graphics::texture const> texture_;
  sprites_type sprites_;
public:
  sample_game() {
  }
  void
  initialize(ebiten::graphics::device::texture_factory_type& tf) {
    NSBundle* bundle = [NSBundle mainBundle];
    NSString* path = [bundle pathForResource:@"test.png" ofType:nil];
    std::string path2([path UTF8String]);
    this->texture_ = tf.from_file(path2);
    typedef ebiten::graphics::drawing_region dr;
    {
      this->sprites_.emplace_back();
      sprite& s = this->sprites_.at(this->sprites_.size() - 1);
      s.drawing_region = dr(0, 0, 32, 32, 32, 32);
    }
    {
      this->sprites_.emplace_back();
      sprite& s = this->sprites_.at(this->sprites_.size() - 1);
      s.drawing_region = dr(0, 0, 32, 132, 32, 32);
    }
    {
      this->sprites_.emplace_back();
      sprite& s = this->sprites_.at(this->sprites_.size() - 1);
      s.drawing_region = dr(0, 0, 132, 32, 32, 32);
    }
    {
      this->sprites_.emplace_back();
      sprite& s = this->sprites_.at(this->sprites_.size() - 1);
      s.drawing_region = dr(0, 0, 132, 132, 32, 32);
    }
    {
      this->sprites_.emplace_back();
      sprite& s = this->sprites_.at(this->sprites_.size() - 1);
      s.drawing_region = dr(0, 0, 32, 32, 32, 32);
    }
    {
      this->sprites_.emplace_back();
      sprite& s = this->sprites_.at(this->sprites_.size() - 1);
      s.drawing_region = dr(0, 0, 64, 32, 32, 32);
    }
  }
  void
  update(int frame_count) {
    // update 中に draw が実行されても困る
    // draw は描画時だけ実行されれば良い
    auto& s = this->sprites_.at(0);
    auto& dr = s.drawing_region;
    dr.set_dst_x(32 + 0.1 * (frame_count % 2400));
    if (frame_count % 600 == 0) {
      std::cout << frame_count << std::endl;
    }
  }
  void
  draw(ebiten::graphics::device::graphics_context_type& gc) const {
    gc.set_texture(texture_->id(),
                   texture_->texture_width(),
                   texture_->texture_height());
    for (auto const& s : this->sprites_) {
      //gc.set_color_matrix(s.color_matrix);
      /*for (auto const& dr : s.drawing_regions) {
        gc.draw(dr);
        }*/
      gc.draw(s.drawing_region);
    }
  }
};

#endif
