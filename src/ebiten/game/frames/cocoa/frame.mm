#include "ebiten/game/frames/cocoa/frame.hpp"
#import "ebiten/game/frames/cocoa/frame.m"
#include <boost/scoped_ptr.hpp>
#include <cassert>

namespace ebiten {
namespace game {
namespace frames {
namespace cocoa {

struct frame::impl {
  boost::scoped_ptr<util::id_> window_;
};

frame::frame(std::size_t width, std::size_t height)
  : pimpl_(new impl) {
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  EbitenWindow* window = [[EbitenWindow alloc]
                          initWithSize:NSMakeSize(width, height)];
  [pool release];
  this->pimpl_->window_.reset(new util::id_(window));
}

frame::~frame() {
}

const util::id_&
frame::native_frame() const {
  return *(this->pimpl_->window_);
}

}
}
}
}
