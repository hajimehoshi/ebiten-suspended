#ifndef EBITEN_SCRIPT_TEXTURE_HOLDERS_HPP
#define EBITEN_SCRIPT_TEXTURE_HOLDERS_HPP

#include "ebiten_script/texture_holder.hpp"
#include "ebiten/graphics/graphics_context.hpp"
#include "ebiten/graphics/texture_factory.hpp"
#include "ebiten/noncopyable.hpp"
#include <unordered_map>


namespace ebiten_script {

class texture_holders : private ebiten::noncopyable {
public:
  typedef std::size_t key_type;
private:
  std::unordered_map<key_type, texture_holder> set_;
  key_type unique_number_;
public:
  texture_holders()
    : unique_number_(0) {
  }
  texture_holders(texture_holders&& rhs)
    : set_(std::move(rhs.set_)),
      unique_number_(std::move(rhs.unique_number_)) {
  }
  template<class... Args>
  key_type
  insert(Args const&... args) {
    key_type key = this->unique_number_;
    this->set_.emplace(key, std::move(texture_holder(args...)));
    ++this->unique_number_;
    return key;
  }
  texture_holder&
  get(key_type const& key) {
    return this->set_.at(key);
  }
  void
  remove(key_type const& key) {
    this->set_.erase(key);
  }
  void
  instantiate(ebiten::graphics::texture_factory& tf) {
    for (auto& p : this->set_) {
      texture_holder& t = p.second;
      if (t.ebiten_texture()) {
        continue;
      }
      t.instantiate(tf);
    }
  }
  void
  flush_drawing_commands(ebiten::graphics::graphics_context&) {
    for (auto& p : this->set_) {
      texture_holder& t = p.second;
      assert(static_cast<bool>(t.ebiten_texture()));
      //t->flush_drawing_commands(g);
    }
  }
};

}

#endif
