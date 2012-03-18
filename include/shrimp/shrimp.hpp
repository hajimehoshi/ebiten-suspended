#ifndef SHRIMP_SHRIMP_HPP
#define SHRIMP_SHRIMP_HPP

#include "shrimp/views/app.hpp"
#include "shrimp/presenters/main_frame.hpp"
#include "ebiten/version.hpp"

int
main() {
  // TODO: Is it nessesary?
  std::cout << "Ebiten" << std::endl
            << "  Commit Date: " << ebiten::version::get_commit_time() << std::endl
            << "  Commit Hash: " << ebiten::version::get_commit_hash() << std::endl
            << "  Commit Modified: " << (ebiten::version::is_commit_modified() ? "Yes" : "No") << std::endl;
  shrimp::views::app app;
  shrimp::presenters::main_frame main_frame(app.main_frame());
  return app.run();
}

#endif
