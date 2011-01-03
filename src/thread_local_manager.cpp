#include "thread_local_manager.hpp"

#include "profile.hpp"
#include "time.hpp"

namespace mtkw {

ThreadLocalManager::ThreadLocalManager(const Flags& flags) {
  setFlags(flags);
}

ThreadLocalManager::~ThreadLocalManager() {
}

int ThreadLocalManager::enable(bool e, bool debug_mode) {
  if (_profile) {
    LOG(ERROR) << "Cannot modify 'enabled' flag while profiling.";
    return -1;
  }
  _flags.enabled = e;
  _flags.debug_mode = _flags.enabled ? debug_mode : false;
  return 0;
}

int ThreadLocalManager::disable() {
  if (_profile) {
    LOG(ERROR) << "Cannot modify 'enabled' flag while profiling.";
    return -1;
  }
  _flags.enabled = false;
  _flags.debug_mode = false;
  return 0;
}

int ThreadLocalManager::setFlags(const Flags& flags) {
  return enable(flags.enabled, flags.debug_mode);
}

int ThreadLocalManager::beginProfile(const std::string& name, bool gen_stat) {
  if (!isEnabled()) return 0;

  ProfilePtr new_prof(new Profile(name, gen_stat));
  if (!_profile) _profile.swap(new_prof);
  else {
    _profile_stack.push_back(_profile);
    _profile->subprofiles.push_back(new_prof);
    _profile = new_prof;
  }

  _profile->start = currentTime();
  return 0;
}

int ThreadLocalManager::endProfile() {
  if (!isEnabled()) return 0;
  if (!_profile) {
    LOG(ERROR) << "No profile to end";
    return -1;
  }

  _profile->end = currentTime();
  _last_profile = _profile;
  if (_profile_stack.empty()) { // root
    _profile.reset();

  } else { // not root
    _profile = _profile_stack.back();
    _profile_stack.pop_back();
  }
  return 0;
}

int ThreadLocalManager::appendProfile(const ProfilePtr& p) {
  if (!_profile) {
    LOG(ERROR) << "No profile to be appended";
    return -1;
  }
  _profile->subprofiles.push_back(p);
  return 0;
}

int ThreadLocalManager::setDebugLog(const std::string& msg) {
  if (!isDebugMode()) return 0;
  if (!_profile) {
    LOG(ERROR) << "No profile to set message";
    return -1;
  }
  _profile->debug_log = msg;
  return 0;
}

int ThreadLocalManager::appendDebugLog(const std::string& msg) {
  if (!isDebugMode()) return 0;
  if (!_profile) {
    LOG(ERROR) << "No profile to append message";
    return -1;
  }
  _profile->debug_log += msg;
  return 0;
}

int ThreadLocalManager::setInformation(const std::string& key, const std::string& value) {
  if (!_profile) {
    LOG(ERROR) << "No profile to append message";
    return -1;
  }
  _profile->information[key] = value;
  return 0;
}

} // namespace mtkw
