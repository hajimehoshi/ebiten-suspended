#ifndef nullptr
#define nullptr (0)
#endif

#include <iostream>

#include "ebiten/game/opengl/device.hpp"
#include "ebiten/game/video/sprite.hpp"
#include "ebiten/game/video/texture.hpp"
#include <deque>
#include <iostream>
#include <stdlib.h>

class sample_game : private boost::noncopyable {
private:
  std::unique_ptr<ebiten::game::video::texture> texture_;
  std::deque<ebiten::game::video::sprite> sprites_;
public:
  template<class texture_factory>
  void
  initialize(texture_factory& tf) {
    this->texture_ = tf.from_file("test.png");
    std::vector<ebiten::game::video::texel_rect> texel_rects;
    this->sprites_.emplace_back(*this->texture_, texel_rects);
  }
  const decltype(sprites_)&
  sprites() {
    return this->sprites_;
  }
  void
  update(int frame_index) {
    auto& geo = this->sprites_[0].geometry_matrix();
    geo.set_ty(geo.ty() + 0.01);
    if (frame_index % 600 == 0) {
      std::cout << "foo!" << std::endl;
    }
  }
  int
  screen_width() const {
    return 320;
  }
  int
  screen_height() const {
    return 240;
  }
};

int
main(int, char**) {
  try {
    sample_game game;
    auto& device = ebiten::game::opengl::device::instance();
    device.run(game, 600, 2);
  } catch (const std::string& message) {
    std::cerr << message << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
