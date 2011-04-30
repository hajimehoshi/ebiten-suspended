#ifndef SAMPLE_GAME_HPP
#define SAMPLE_GAME_HPP

#include "ebiten/ebiten.hpp"
#include <boost/foreach.hpp>
#include <boost/optional.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/range.hpp>
#include <cstdlib>
#include <deque>
#include <iostream>

class sample_game : private boost::noncopyable {
private:
  typedef ebiten::game::graphics::sprite sprite_type;
  typedef boost::ptr_vector<sprite_type> sprites_type;
  typedef ebiten::game::graphics::drawing_region drawing_region_type;
  boost::optional<ebiten::game::graphics::texture> texture_;
  sprites_type sprites_;
public:
  explicit
  sample_game(ebiten::game::graphics::device::texture_factory_type& tf) {
    // TODO: カレントディレクトリについてどうにかする
    this->texture_ = boost::in_place(tf.from_file("/Users/hajime/ebiten/test.png"));
    this->sprites_.push_back(new sprite_type(this->texture_.get(), 4));
    sprite_type& s = this->sprites_.at(0);
    s.geometry_matrix().set_a(1);
    BOOST_FOREACH(drawing_region_type& dr, s.drawing_regions()) {
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
    s.color_matrix().set_element<0, 3>(1);
  }
  sprites_type&
  sprites() {
    return this->sprites_;
  }
  sprites_type const&
  sprites() const {
    return this->sprites_;
  }
  void
  update(int frame_count) {
    ebiten::game::graphics::drawing_region& dr =
      this->sprites_.at(0).drawing_region_at(0);
    dr.set_dst_x(dr.dst_x() + 0.01);
    if (frame_count % 600 == 0) {
      std::cout << "foo!" << std::endl;
    }
  }
};

#endif
