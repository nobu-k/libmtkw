#ifndef LIBMTKW_SRC_PROFILE_HPP_
#define LIBMTKW_SRC_PROFILE_HPP_

#include <string>
#include <vector>
#include "util.hpp"

namespace mtkw {

struct Profile;
// TODO: support thread_model of pfi::lang::shared_ptr
typedef shared_ptr<Profile> ProfilePtr;

struct Profile {
  std::string name;
  double start;
  double end;

  std::string message;

  ProfilePtr parent;
  std::vector<ProfilePtr> subprofiles;

  bool isRoot() const { return !parent; }
};

} // namespace mtkw

#endif
