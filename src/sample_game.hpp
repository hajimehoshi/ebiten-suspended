#ifndef SAMPLE_GAME_HPP
#define SAMPLE_GAME_HPP

#include "ebiten/game/graphics/drawing_region.hpp"
#include "ebiten/game/graphics/sprite.hpp"
#include "ebiten/game/graphics/texture.hpp"
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include <boost/range.hpp>
#include <boost/shared_ptr.hpp>
#include <cstdlib>
#include <deque>
#include <iostream>

class sample_game : private boost::noncopyable {
  typedef ebiten::game::graphics::sprite sprite_t;
  typedef boost::shared_ptr<sprite_t> sprite_ptr_t;
  typedef std::deque<sprite_ptr_t> sprites_t;
public:
  template<class TextureFactory>
  void
  initialize(TextureFactory& tf) {
    this->texture_ = tf.from_file("test.png");
    this->sprites_.push_back(boost::make_shared<sprite_t>(*this->texture_, 4));
    sprite_ptr_t& s = this->sprites_.at(0);
    s->geometry_matrix().set_a(1);
    typedef ebiten::game::graphics::drawing_region drawing_region_t;
    const sprite_t::drawing_regions_t& drs = s->drawing_regions();
    BOOST_FOREACH(const boost::range_value<sprite_t::drawing_regions_t>::type& dr, drs) {
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
  sprites_t&
  sprites() {
    return this->sprites_;
  }
  const sprites_t&
  sprites() const {
    return this->sprites_;
  }
  void
  update(int frame_count) {
    BOOST_AUTO(&drawing_region, this->sprites_.at(0)->drawing_region_at(0));
    drawing_region.set_dst_x(drawing_region.dst_x() + 0.01);
    if (frame_count % 600 == 0) {
      std::cout << "foo!" << std::endl;
    }
  }
private:
  boost::shared_ptr<ebiten::game::graphics::texture> texture_;
  sprites_t sprites_;
};

#endif
