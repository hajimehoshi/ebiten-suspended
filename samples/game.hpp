#ifndef GAME_HPP
#define GAME_HPP

#include "ebiten/ebiten.hpp"
#include "ebiten/image.hpp"
#include "ebiten/image_loader.hpp"
#include "ebiten/resources.hpp"

class game : private ebiten::noncopyable {
private:
  ebiten::graphics::texture ebiten_texture_;
public:
  bool
  update(ebiten::graphics::texture_factory& tf,
         ebiten::input const&) {
    if (!this->ebiten_texture_) {
      std::string path = ebiten::get_resource_path("ebiten.png");
      // TODO: image should be movable?
      std::unique_ptr<ebiten::image> image = ebiten::image_loader::load_png(path);
      this->ebiten_texture_ = tf.from_image(*image);
    }
    return false;
  }
  void
  draw(ebiten::graphics::graphics_context& g,
       ebiten::graphics::texture& offscreen) {
    g.set_offscreen(offscreen);
    g.fill(128, 128, 255, 255);
    g.draw_texture(this->ebiten_texture_,
                   0, 0, 64, 32,
                   ebiten::graphics::geometry_matrix::identity(),
                   ebiten::graphics::color_matrix::identity());
  }
};

#endif
