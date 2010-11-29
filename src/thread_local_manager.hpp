#ifndef LIBMTKW_SRC_THREAD_LOCAL_MANAGER_HPP_
#define LIBMTKW_SRC_THREAD_LOCAL_MANAGER_HPP_

#include <string>
#include "util.hpp"
#include "profile.hpp"

namespace mtkw {

class ThreadLocalManager : noncopyable {
private:
  bool _enabled;
  ProfilePtr _profile;
  ProfilePtr _last_profile;

private:
  ProfilePtr& getCurrentProfile(const std::string& root_name);

public:
  ThreadLocalManager();
  ~ThreadLocalManager();

  int enable(bool e = true);
  int disable();
  bool isEnabled() const { return _enabled; }

  int beginProfile(const std::string& name, bool gen_stat = false);
  int endProfile();
  int appendProfile(const ProfilePtr& p);

  int setDebugLog(const std::string& msg);
  int appendDebugLog(const std::string& msg);

  ProfilePtr getCurrentProfile() const { return _profile; }
  ProfilePtr getLastProfile() const { return _last_profile; }
};

} // namespace mtkw

#endif
