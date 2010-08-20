#include "thread_local_manager.hpp"

#include "profile.hpp"

namespace mtkw {

int ThreadLocalManager::enable(bool e)
{
  if (_profile) {
    LOG(ERROR) << "Cannot modify 'enabled' flag while profiling.";
    return -1;
  }
  _enabled = e;
  return 0;
}

int ThreadLocalManager::disable()
{
  if (_profile) {
    LOG(ERROR) << "Cannot modify 'enabled' flag while profiling.";
    return -1;
  }
  _enabled = false;
  return 0;
}

int ThreadLocalManager::beginProfile(const std::string& name)
{
  if (!_enabled) return 0;

  ProfilePtr new_prof(new Profile(name));
  if (!_profile) _profile.swap(new_prof);
  else {
    new_prof.parent = _profile;
    _profile.subprofiles.push_back(new_profile);
    _profile = new_profile;
  }

  _profile->start = currentTime();
  return 0;
}

int ThreadLocalManager::endProfile()
{
  if (!_enabled) return 0;
  if (!_profile) {
    LOG(ERROR) << "No profile to end";
    return -1;
  }

  _profile->end = currentTime();
  if (_profile->parent) { // not root
    _profile = _profile->parent;

  } else { // root
    _last_profile = _profile;
    _profile.reset();
  }
  return 0;
}

int ThreadLocalManager::appendProfile(const ProfilePtr& p)
{
  if (!_profile) {
    LOG(ERROR) << "No profile to be appended";
    return -1;
  }
  _profile->subprofiles.push_back(p);
  return 0;
}

} // namespace mtkw
