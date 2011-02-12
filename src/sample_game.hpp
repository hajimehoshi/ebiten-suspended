#ifndef SAMPLE_GAME_HPP
#define SAMPLE_GAME_HPP

#include "ebiten/game/graphics/drawing_region.hpp"
#include "ebiten/game/graphics/sprite.hpp"
#include "ebiten/game/graphics/texture.hpp"
#include <cstdlib>
#include <deque>
#include <iostream>
#include <memory>

class sample_game : private boost::noncopyable {
private:
  std::unique_ptr<ebiten::game::graphics::texture> texture_;
  std::deque<ebiten::game::graphics::sprite> sprites_;
public:
  template<class TextureFactory>
  void
  initialize(TextureFactory& tf) {
    this->texture_ = tf.from_file("test.png");
    typedef ebiten::game::graphics::drawing_region drawing_region;
    this->sprites_.emplace_back(*this->texture_, 4);
    // TODO: modify API
    auto& s = this->sprites_.at(0);
    s.geometry_matrix().set_a(1);
    s.drawing_region_at(0) = std::move(drawing_region(0, 0, 32, 32, 32, 32));
    s.drawing_region_at(1) = std::move(drawing_region(0, 0, 132, 32, 32, 32));
    s.drawing_region_at(2) = std::move(drawing_region(0, 0, 32, 132, 32, 32));
    s.drawing_region_at(3) = std::move(drawing_region(0, 0, 132, 132, 32, 32));
    s.color_matrix().set_element<0, 0>(0);
    s.color_matrix().set_element<0, 3>(1);
  }
  const decltype(sprites_)&
  sprites() {
    return this->sprites_;
  }
  void
  update(int frame_count) {
    auto& drawing_region = this->sprites_.at(0).drawing_region_at(0);
    drawing_region.dst_x += 0.01;
    if (frame_count % 600 == 0) {
      std::cout << "foo!" << std::endl;
    }
  }
};

#endif

