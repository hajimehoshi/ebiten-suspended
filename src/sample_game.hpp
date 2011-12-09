#ifndef SAMPLE_GAME_HPP
#define SAMPLE_GAME_HPP

#include "ebiten/ebiten.hpp"
#include "ebiten/image_loader.hpp"
#include <cstdlib>
#include <iostream>

#include "yield.hpp"

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
  coroutine c;
  int i;
public:
  sample_game() {
  }
  void
  initialize(ebiten::graphics::device::texture_factory_type& tf) {
    NSBundle* bundle = [NSBundle mainBundle];
    NSString* path = [bundle pathForResource:@"test.png" ofType:nil];
    std::string path2([path UTF8String]);
    // TODO: Load Async
    ebiten::image image(ebiten::png_image_loader, path2);
    // TODO: Texture Atlas
    auto t = tf.from_image(image);
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
  update(int) {
    // update 中に draw が実行されても困る
    // draw は描画時だけ実行されれば良い
    auto& s = this->sprites_.at(0);
    auto& dr = s.drawing_region;
    reenter(c) {
      for (;;) {
        for (i = 0; i < 2400; i++) {
          dr.dst_x = 32 + 0.1 * (i % 2400);
          yield();
        }
        for (i = 2400; 0 <= i; i--) {
          dr.dst_x = 32 + 0.1 * (i % 2400);
          yield();
        }
      }
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

#include "unyield.hpp"

#endif
