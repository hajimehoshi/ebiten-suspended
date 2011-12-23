#ifndef SAMPLES_SAMPLE_GAME_HPP
#define SAMPLES_SAMPLE_GAME_HPP

#include "ebiten/ebiten.hpp"
#include "ebiten/image_loader.hpp"
#include <cstdlib>
#include <future>
#include <iostream>

#include "yield.hpp"

class sprite {
public:
  double x;
  double y;
  sprite()
    : x(0), y(0) {
  }
  sprite(double x, double y)
    : x(x), y(y) {
  }
};

class sample_game : private ebiten::noncopyable {
private:
  typedef std::vector<sprite> sprites_type;
  ebiten::graphics::texture texture_;
  sprites_type sprites_;
  coroutine c;
  std::future<std::unique_ptr<ebiten::image>> image_;
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
        {
          NSBundle* bundle = [NSBundle mainBundle];
          NSString* ns_path = [bundle pathForResource:@"test.png" ofType:nil];
          if (ns_path != nil) {
            std::string path([ns_path UTF8String]);
            this->image_ = std::async(ebiten::image_loader::load_png, path);
          }
        }
        while (!this->image_.valid()) {
          yield();
        }
        // TODO: Texture Atlas
        this->texture_ = tf.from_image(*(this->image_.get()));
      }
      {
        this->sprites_.at(0) = sprite(32, 32);
        this->sprites_.at(1) = sprite(32, 132);
        this->sprites_.at(2) = sprite(132, 32);
        this->sprites_.at(3) = sprite(132, 132);
        this->sprites_.at(4) = sprite(32, 32);
        this->sprites_.at(5) = sprite(64, 32);
      }
      {
        for (;;) {
          for (this->i = 0; this->i < 240; ++this->i) {
            {
              auto& s = this->sprites_.at(0);
              s.x = 32 + (this->i % 240);
            }
            yield();
          }
          for (this->i = 240 - 1; 0 <= this->i; --this->i) {
            {
              auto& s = this->sprites_.at(0);
              s.x = 32 + (this->i % 240);
            }
            yield();
          }
        }
      }
    }
  }
  void
  draw(ebiten::graphics::graphics_context& g) const {
    if (!this->texture_) {
      // loading...
      return;
    }
    g.draw_rect(0, 0, 100, 100, 51, 102, 153, 128);
    
    int i = 0;
    for (auto const& s : this->sprites_) {
      auto geo_mat   = ebiten::graphics::geometry_matrix::identity();
      auto color_mat = ebiten::graphics::color_matrix::identity();
      if (i == 5) {
        geo_mat = ebiten::graphics::geometry_matrix(2, 0, 0, 2, 0, 0);
      } else if (i == 4) {
        color_mat.set_element<0, 0>(0);
      }
      g.draw_texture(this->texture_,
                     0, 0, 32, 32, s.x, s.y,
                     geo_mat, color_mat);
      ++i;
    }
  }
};

#include "unyield.hpp"

#endif
