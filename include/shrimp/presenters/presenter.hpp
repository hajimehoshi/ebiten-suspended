#ifndef SHRIMP_PRESENTERS_PRESENTER_HPP
#define SHRIMP_PRESENTERS_PRESENTER_HPP

#include "shrimp/views/view.hpp"
#include "ebiten/noncopyable.hpp"

namespace shrimp {
namespace presenters {

class presenter : private ebiten::noncopyable {
private:
  views::view& view_;
public:
  presenter(views::view& view)
    : view_(view) {
  }
};

}
}

#endif
