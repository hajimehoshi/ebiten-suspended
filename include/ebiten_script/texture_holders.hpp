#ifndef EBITEN_SCRIPT_TEXTURE_HOLDERS_HPP
#define EBITEN_SCRIPT_TEXTURE_HOLDERS_HPP

#include "ebiten_script/texture_holder.hpp"
#include "ebiten_script/texture_command.hpp"
#include "ebiten/graphics/graphics_context.hpp"
#include "ebiten/graphics/texture_factory.hpp"
#include "ebiten/noncopyable.hpp"
#include <unordered_map>
#include <vector>

namespace ebiten_script {

class texture_holders : private ebiten::noncopyable {
public:
  typedef std::size_t key_type;
private:
  std::unordered_map<key_type, texture_holder> map_;
  key_type unique_number_;
  std::vector<std::unique_ptr<texture_command> > commands_;
public:
  texture_holders()
    : unique_number_(0) {
  }
  explicit
  texture_holders(texture_holders&& rhs)
    : map_(std::move(rhs.map_)),
      unique_number_(std::move(rhs.unique_number_)) {
  }
  template<class... Args>
  key_type
  insert(Args const&... args) {
    key_type key = this->unique_number_;
    //std::cout << "insert: " << key << std::endl;
    this->map_.emplace(key, std::move(texture_holder(args...)));
    ++this->unique_number_;
    return key;
  }
  texture_holder&
  get(key_type const& key) {
    //std::cout << "get: " << key << std::endl;
    assert(this->map_.find(key) != this->map_.end());
    return this->map_.at(key);
  }
  texture_holder const&
  get(key_type const& key) const {
    //std::cout << "get: " << key << std::endl;
    assert(this->map_.find(key) != this->map_.end());
    return this->map_.at(key);
  }
  void
  remove(key_type const& key) {
    //std::cout << "remove: " << key << std::endl;
    this->map_.erase(key);
  }
  void
  instantiate(ebiten::graphics::texture_factory& tf) {
    for (auto& p : this->map_) {
      texture_holder& t = p.second;
      if (t.is_instantiated()) {
        continue;
      }
      t.instantiate(tf);
    }
  }
  void
  add_texture_command(std::unique_ptr<texture_command>& command) {
    this->commands_.push_back(std::move(command));
  }
  void
  flush_texture_commands(ebiten::graphics::graphics_context& g) {
    for (auto& command : this->commands_) {
      command->exec(g);
    }
    this->commands_.clear();
  }
};

}

#endif
