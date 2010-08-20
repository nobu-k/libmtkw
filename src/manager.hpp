#ifndef LIBMTKW_SRC_MANAGER_HPP_
#define LIBMTKW_SRC_MANAGER_HPP_

#include "util.hpp"

namespace mtkw {

class Manager {
private:
  Manager();
  static Manager* _instance;

  shared_ptr<ThreadLocalManager> _tl_manager;

public:
  ~Manager();

  /**
   * @name Profiling functions
   */
  //@{
  int enable(bool e = true);
  int disable();
  int beginProfile(const std::string& name);
  int endProfile();
  int appendProfile(const ProfilePtr& p);
  //@}

  static Manager& instance() {
    // TODO: make this thread-safe
    if (_instance == NULL) {
      _instance = new Manager();
    }
    return *_instance;
  }
};

} // namespace mtkw

#endif
