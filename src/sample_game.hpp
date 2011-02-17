#ifndef SAMPLE_GAME_HPP
#define SAMPLE_GAME_HPP

#include "ebiten/game/graphics/drawing_region.hpp"
#include "ebiten/game/graphics/sprite.hpp"
#include "ebiten/game/graphics/texture.hpp"
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include <boost/optional.hpp>
#include <boost/range.hpp>
#include <boost/shared_ptr.hpp>
#include <cstdlib>
#include <deque>
#include <iostream>

class sample_game : private boost::noncopyable {
private:
  typedef ebiten::game::graphics::sprite sprite_type;
  typedef boost::shared_ptr<sprite_type> sprite_ptr_type;
  typedef std::deque<sprite_ptr_type> sprites_type;
  boost::optional<ebiten::game::graphics::texture> texture_;
  sprites_type sprites_;
public:
  template<class TextureFactory>
  void
  initialize(TextureFactory& tf) {
    this->texture_ = boost::in_place(tf.from_file("test.png"));
    this->sprites_.push_back(boost::make_shared<sprite_type>(this->texture_.get(), 4));
    sprite_ptr_type& s = this->sprites_.at(0);
    s->geometry_matrix().set_a(1);
    sprite_type::drawing_regions_type const& drs = s->drawing_regions();
    BOOST_FOREACH(boost::range_value<sprite_type::drawing_regions_type>::type const& dr, drs) {
      dr->set_width(32);
      dr->set_height(32);
    }
    s->drawing_region_at(0).set_dst_x(32);
    s->drawing_region_at(0).set_dst_y(32);
    s->drawing_region_at(1).set_dst_x(32);
    s->drawing_region_at(1).set_dst_y(132);
    s->drawing_region_at(2).set_dst_x(132);
    s->drawing_region_at(2).set_dst_y(32);
    s->drawing_region_at(3).set_dst_x(132);
    s->drawing_region_at(3).set_dst_y(132);
    s->color_matrix().set_element<0, 0>(0);
    s->color_matrix().set_element<0, 3>(1);
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
      this->sprites_.at(0)->drawing_region_at(0);
    dr.set_dst_x(dr.dst_x() + 0.01);
    if (frame_count % 600 == 0) {
      std::cout << "foo!" << std::endl;
    }
  }
};

#endif
