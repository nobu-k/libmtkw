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

  std::string debug_log;

  std::vector<ProfilePtr> subprofiles;

  explicit Profile(const std::string& name, bool gen_stat = false)
    : name(name), generate_statistics(gen_stat), start(0), end(0) {}
  double time() const { return end - start; }

  void getStatistics(ProfileStatistics& statistics) const;
  void simpleFormat(std::ostream& out,
                    const std::string& indent = "  ",
                    const std::string& initial_indent = "") const;
  std::string simpleFormat(const std::string& indent = "  ",
                           const std::string& initial_indent = "") const;
};

/**
 * @note This class is not thread safe.
 */
class SingleProfileStatistics {
private:
  size_t _called;
  double _total;
  double _max;
  double _min;

public:
  SingleProfileStatistics();
  ~SingleProfileStatistics();

  void add(const Profile& prof);

  size_t called() const { return _called; }
  double average() const { return _called ? _total / _called : 0; }
  double max() const { return _max; }
  double min() const { return _min; }

  void swap(SingleProfileStatistics& s);
};

class ProfileStatistics : noncopyable {
private:
  struct Statistics {
    SingleProfileStatistics stat;
    mutable thread::rw_mutex mutex;

    Statistics() {}
    Statistics(const Statistics& st)
      : stat(st.stat) {} // not copy mutex
  };

  typedef std::map<std::string, Statistics> StatisticsMap;
  StatisticsMap _statistics;

  // This mutex only guards the state of _statistics.
  // Statistics::stat can be modified even if _mutex
  // is write-locked.
  mutable thread::rw_mutex _mutex;

  void addImpl(Statistics& st, const Profile& prof);

public:
  ProfileStatistics();
  ~ProfileStatistics();

  size_t size() const;

  /**
   * @note This function does not add subprofiles of prof.
   */
  void add(const Profile& prof);

  int get(const std::string& name, SingleProfileStatistics& result) const;
  void getAll(std::map<std::string, SingleProfileStatistics>& result) const;

  void clear();
};

} // namespace mtkw

#endif
