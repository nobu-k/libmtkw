#ifndef LIBMTKW_SRC_THREAD_LOCAL_MANAGER_HPP_
#define LIBMTKW_SRC_THREAD_LOCAL_MANAGER_HPP_

#include <string>
#include "util.hpp"

namespace mtkw {

struct Profile;

class ThreadLocalManager {
private:
  bool _enabled;
  ProfilePtr _profile;
  ProfilePtr _last_profile;

private:
  ProfilePtr& getCurrentProfile(const std::string& root_name);

public:
  int enable(bool e = true);
  int disable();

  int beginProfile(const std::string& name);
  int endProfile();
  int appendProfile(const ProfilePtr& p);

  ProfilePtr getLastProfile() const {
    return _last_profile;
  }
};

} // namespace mtkw

#endif
