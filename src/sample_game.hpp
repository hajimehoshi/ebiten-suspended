#ifndef SAMPLE_GAME_HPP
#define SAMPLE_GAME_HPP

#include "ebiten/ebiten.hpp"
#include <boost/ptr_container/ptr_vector.hpp>
#include <cstdlib>
#include <deque>
#include <iostream>

class sample_game : private ebiten::util::noncopyable {
private:
  typedef ebiten::graphics::sprite sprite_type;
  typedef boost::ptr_vector<sprite_type> sprites_type;
  typedef ebiten::graphics::drawing_region drawing_region_type;
  std::unique_ptr<ebiten::graphics::texture const> texture_;
  sprites_type sprites_;
public:
  sample_game() {
  }
  void
  initialize(ebiten::graphics::device::texture_factory_type& tf) {
    // TODO: カレントディレクトリについてどうにかする
    this->texture_ = tf.from_file("/Users/hajime/ebiten/test.png");
    {
      this->sprites_.push_back(new sprite_type(*this->texture_, 4));
      sprite_type& s = this->sprites_.at(0);
      for (auto& dr : s.drawing_regions()) {
        dr.set_width(32);
        dr.set_height(32);
      }
      s.drawing_region_at(0).set_dst_x(32);
      s.drawing_region_at(0).set_dst_y(32);
      s.drawing_region_at(1).set_dst_x(32);
      s.drawing_region_at(1).set_dst_y(132);
      s.drawing_region_at(2).set_dst_x(132);
      s.drawing_region_at(2).set_dst_y(32);
      s.drawing_region_at(3).set_dst_x(132);
      s.drawing_region_at(3).set_dst_y(132);
      s.color_matrix().set_element<0, 0>(0);
      s.color_matrix().set_element<0, 3>(0);
    }
    {
      this->sprites_.push_back(new sprite_type(*this->texture_, 1));
      sprite_type& s = this->sprites_.at(1);
      s.drawing_region_at(0).set_width(32);
      s.drawing_region_at(0).set_height(32);
      s.drawing_region_at(0).set_dst_x(32);
      s.drawing_region_at(0).set_dst_y(32);
    }
    {
      this->sprites_.push_back(new sprite_type(*this->texture_, 1));
      sprite_type& s = this->sprites_.at(2);
      s.drawing_region_at(0).set_width(32);
      s.drawing_region_at(0).set_height(32);
      s.drawing_region_at(0).set_dst_x(64);
      s.drawing_region_at(0).set_dst_y(32);
    }
  }
  void
  update(int frame_count) {
    // update 中に draw が実行されても困る
    // draw は描画時だけ実行されれば良い
    ebiten::graphics::drawing_region& dr =
      this->sprites_.at(0).drawing_region_at(0);
    dr.set_dst_x(32 + 0.1 * (frame_count % 2400));
    if (frame_count % 600 == 0) {
      std::cout << frame_count << std::endl;
    }
  }
  void
  draw(ebiten::graphics::device::graphics_context_type& gc) const {
    for (auto const& s : this->sprites_) {
      gc.draw(s);
    }
  }
};

#endif
