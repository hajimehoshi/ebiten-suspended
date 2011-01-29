#ifndef nullptr
#define nullptr (0)
#endif

#ifdef EBITEN_TEST
#include <gtest/gtest.h>
#endif

#include "ebiten/game/graphics/drawing_region.hpp"
#include "ebiten/game/graphics/sprite.hpp"
#include "ebiten/game/graphics/texture.hpp"
#include "ebiten/game/kernels/macosx_kernel.hpp"
#include <cstdlib>
#include <deque>
#include <iostream>

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
  update(int frame_index) {
    auto& drawing_region = this->sprites_.at(0).drawing_region_at(0);
    drawing_region.dst_x += 0.01;
    if (frame_index % 600 == 0) {
      std::cout << "foo!" << std::endl;
    }
  }
};

int
main(int argc, char** argv) {
  (void)argc;
  (void)argv;
#ifdef EBITEN_TEST
  testing::InitGoogleTest(&argc, argv);
  const auto test_result = RUN_ALL_TESTS();
  if (test_result != EXIT_SUCCESS) {
    return test_result;
  }
  if (2 <= argc) {
    return EXIT_SUCCESS;
  }
#endif
  try {
    sample_game game;
    auto& kernel = ebiten::game::kernels::macosx_kernel::instance();
    kernel.run(game, 320, 240, 600, 2);
  } catch (const std::string& message) {
    // TODO: use boost::diagnostic_information?
    std::cerr << message << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
