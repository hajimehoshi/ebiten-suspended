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
  ebiten::graphics::texture texture_;
  sprites_type sprites_;
  coroutine c;
  int i;
public:
  sample_game() {
    for (int i = 0; i < 6; ++i) {
      this->sprites_.emplace_back();
    }
  }
  void
  update(ebiten::graphics::texture_factory& tf) {
    reenter(c) {
      {
        NSBundle* bundle = [NSBundle mainBundle];
        NSString* path = [bundle pathForResource:@"test.png" ofType:nil];
        std::string path2([path UTF8String]);
        // TODO: Load Async
        ebiten::image image(ebiten::png_image_loader, path2);
        // TODO: Texture Atlas
        this->texture_ = tf.from_image(image);
      }
      {
        typedef ebiten::graphics::drawing_region dr;
        this->sprites_.at(0).drawing_region = dr(0, 0, 32, 32, 32, 32);
        this->sprites_.at(1).drawing_region = dr(0, 0, 32, 132, 32, 32);
        this->sprites_.at(2).drawing_region = dr(0, 0, 132, 32, 32, 32);
        this->sprites_.at(3).drawing_region = dr(0, 0, 132, 132, 32, 32);
        this->sprites_.at(4).drawing_region = dr(0, 0, 32, 32, 32, 32);
        this->sprites_.at(5).drawing_region = dr(0, 0, 64, 32, 32, 32);
      }
      {
        for (;;) {
          for (this->i = 0; this->i < 240; ++this->i) {
            {
              auto& s = this->sprites_.at(0);
              auto& dr = s.drawing_region;
              dr.dst_x = 32 + (this->i % 240);
            }
            yield();
          }
          for (this->i = 240 - 1; 0 <= this->i; --this->i) {
            {
              auto& s = this->sprites_.at(0);
              auto& dr = s.drawing_region;
              dr.dst_x = 32 + (this->i % 240);
            }
            yield();
          }
        }
      }
    }
  }
  void
  draw(ebiten::graphics::graphics_context& gc) const {
    if (!this->texture_) {
      // loading...
      return;
    }
    gc.draw_rect(0, 0, 100, 100, 51, 102, 153, 204);
    
    gc.set_texture(this->texture_);
    int i = 0;
    for (auto const& s : this->sprites_) {
      if (i == 5) {
        auto mat = ebiten::graphics::geometry_matrix(2, 0, 0, 2, 0, 0);
        gc.set_geometry_matrix(mat);
      } else if (i == 4) {
        ebiten::graphics::color_matrix mat;
        mat.set_identity();
        mat.set_element<0, 0>(0);
        gc.set_color_matrix(mat);
      } else {
        auto mat = ebiten::graphics::geometry_matrix(1, 0, 0, 1, 0, 0);
        gc.set_geometry_matrix(mat);
      }
      gc.draw(s.drawing_region);
      ++i;
    }
  }
};

#include "unyield.hpp"

#endif
