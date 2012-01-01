#ifndef EBITEN_SCRIPT_TEXTURE_COMMAND_HPP
#define EBITEN_SCRIPT_TEXTURE_COMMAND_HPP

#include "ebiten_script/texture_holder.hpp"
#include "ebiten/graphics/graphics_context.hpp"
#include "ebiten/graphics/texture.hpp"
#include "ebiten/noncopyable.hpp"

namespace ebiten_script {

class texture_command : private ebiten::noncopyable {
private:
  // TODO: change texture to texture_holder
  texture_holder& texture_holder_;
public:
  texture_command(texture_holder& texture_holder)
    : texture_holder_(texture_holder) {
  }
  virtual
  ~texture_command() {
  }
  virtual void
  exec(ebiten::graphics::graphics_context& g) {
    if (!this->texture_holder_.ebiten_texture()) {
      throw std::runtime_error("the texture is not created");
    }
    this->do_exec(g);
  }
  class texture_holder&
  texture_holder() {
    return this->texture_holder_;
  }
  class texture_holder const&
  texture_holder() const {
    return this->texture_holder_;
  }
protected:
  virtual void
  do_exec(ebiten::graphics::graphics_context&) = 0;
};

class texture_command_clear : public texture_command {
public:
  void
  do_exec(ebiten::graphics::graphics_context& g) {
    ebiten::graphics::texture& t = this->texture_holder().ebiten_texture();
    g.set_offscreen(t);
    g.clear();
  }
};

class texture_command_draw_rect : public texture_command {
private:
  std::size_t x_, y_, width_, height_;
  uint8_t red_, green_, blue_, alpha_;
public:
  texture_command_draw_rect(class texture_holder& texture_holder,
                            std::size_t x,
                            std::size_t y,
                            std::size_t width,
                            std::size_t height,
                            uint8_t red,
                            uint8_t green,
                            uint8_t blue,
                            uint8_t alpha)
    : texture_command(texture_holder),
      x_(x), y_(y), width_(width), height_(height),
      red_(red), green_(green), blue_(blue), alpha_(alpha) {
  }
  void
  do_exec(ebiten::graphics::graphics_context& g) {
    ebiten::graphics::texture& t = this->texture_holder().ebiten_texture();
    g.set_offscreen(t);
    g.draw_rect(this->x_, this->y_, this->width_, this->height_,
                this->red_, this->green_, this->blue_, this->alpha_);
  }
};

}

#endif

