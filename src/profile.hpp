#ifndef LIBMTKW_SRC_PROFILE_HPP_
#define LIBMTKW_SRC_PROFILE_HPP_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "util.hpp"
#include "thread.hpp"

namespace mtkw {

struct Profile;
class ProfileStatistics;

// TODO: support thread_model of pfi::lang::shared_ptr
typedef shared_ptr<Profile> ProfilePtr;

struct Profile {
  const std::string name;
  const bool generate_statistics;
  double start;
  double end;

  std::string message;

  ProfilePtr parent;
  std::vector<ProfilePtr> subprofiles;

  explicit Profile(const std::string& name, bool gen_stat = false)
    : name(name), generate_statistics(gen_stat), start(0), end(0) {}
  bool isRoot() const { return !parent; }
  double time() const { return end - start; }

  void getStatistics(std::map<std::string, ProfileStatistics>& statistics) const;
  void simpleFormat(std::ostream& out,
                    const std::string& indent = "  ",
                    const std::string& initial_indent = "") const;
  std::string simpleFormat(const std::string& indent = "  ",
                           const std::string& initial_indent = "") const;
};

class ProfileStatistics {
private:
  size_t _called;
  double _total;
  double _max;
  double _min;

  /// @todo This should be hidden in pimpl
  mutable thread::rw_mutex _mutex;

  void swapImpl(ProfileStatistics& s);

public:
  ProfileStatistics();
  ProfileStatistics(const ProfileStatistics& s);
  ~ProfileStatistics();

  ProfileStatistics& operator =(const ProfileStatistics& s);

  void add(const Profile& prof);

  size_t called() const;
  double average() const;
  double max() const;
  double min() const;

  void swap(ProfileStatistics& s);
};

} // namespace mtkw

#endif
