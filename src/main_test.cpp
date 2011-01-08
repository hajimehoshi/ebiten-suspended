#define BOOST_TEST_MODULE "Ebiten"
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "ebiten/game/opengl/texture_factory.hpp"
#include "ebiten/game/video/affine_matrix.hpp"
#include "ebiten/game/video/color.hpp"
#include "ebiten/game/video/color_matrix.hpp"
#include "ebiten/game/video/geometry_matrix.hpp"
#include "ebiten/game/video/texture.hpp"
#include "ebiten/util/channel.hpp"
#include "ebiten/util/image_loader.hpp"
