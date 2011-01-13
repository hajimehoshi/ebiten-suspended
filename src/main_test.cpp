#define EBITEN_TEST

#include <gtest/gtest.h>

#include "ebiten/game/opengl/texture_factory.hpp"
#include "ebiten/game/video/affine_matrix.hpp"
#include "ebiten/game/video/geometry_matrix.hpp"
#include "ebiten/util/channel.hpp"
#include "ebiten/util/image_loader.hpp"

#include <cstdlib>

int
main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
