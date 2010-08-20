#ifndef LIBMTKW_SRC_PROFILE_HPP_
#define LIBMTKW_SRC_PROFILE_HPP_

#include <string>
#include <vector>
#include "util.hpp"

namespace mtkw {

struct IndividualProfile {
  std::string name;
};

struct MasterProfile {
  std::string name;
};

struct Profile;
typedef shared_ptr<Profile> ProfilePtr;

struct Profile {
  std::string name;
  double start;
  double end;

  ProfilePtr parent;
  std::list<ProfilePtr> subprofiles;

  bool isRoot() const { return !parent; }
};

} // namespace mtkw

#endif
