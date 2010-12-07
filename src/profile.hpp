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

struct Profile;
class ProfileVisitor {
public:
  virtual ~ProfileVisitor() {}

  /**
   * This function is called by Profile::accept.
   * If Profile had subprofiles, this function is
   * called for each subprofile as well.
   *
   * @param profile A Profile which called visit.
   */
  virtual void visit(const Profile& profile) = 0;
};

/**
 * @note This class has to traverse Profile by itself.
 * This can be done by calling Profile::iterateSubprofiles.
 */
class ProfileTraversingVisitor {
public:
  virtual ~ProfileTraversingVisitor() {}
  virtual void visit(const Profile& profile) = 0;
};

struct Profile {
  const std::string name; //!< The name (or id) of this profile.
  const bool generate_statistics; //!< Generate statistics for this profile?
  double start; //!< The time this profile started.
  double end; //!< The time this profile ended.

  std::string debug_log; //!< Debug logs.
  std::map<std::string, std::string> information; //!< Key-value information
  std::vector<ProfilePtr> subprofiles; //!< Subprofiles.

  /**
   * @param name The name (or id) of this profile.
   * @param gen_stat Pass true when generating statistics of this profile.
   */
  explicit Profile(const std::string& name, bool gen_stat = false)
    : name(name), generate_statistics(gen_stat), start(0), end(0) {}

  /**
   * @brief Returns the time consumed by this profile.
   */
  double time() const { return end - start; }

  /**
   * @brief Accept ProfileVisitor
   */
  void accept(ProfileVisitor& visitor) const;

  /**
   * @brief Accept ProfileTraversingVisitor
   */
  void accept(ProfileTraversingVisitor& visitor) const;

  /**
   * @brief Call accept(visitor) of each subprofile.
   */
  void iterateSubprofiles(ProfileTraversingVisitor& visitor) const;

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
  size_t _called; //!< The number of calls (executions) of this profile.
  double _total; //!< The total time consumed by this profile.
  double _max; //!< The longest time.
  double _min; //!< The shortest time.

public:
  SingleProfileStatistics();
  ~SingleProfileStatistics();

  /**
   * @brief Add a profile.
   *
   * @param prof A profile to be added.
   *
   * @note All profiles added to this class should have the same name(id).
   */
  void add(const Profile& prof);

  size_t called() const { return _called; }

  /**
   * @brief Returns the average consumed time.
   */
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
  void addAll(const Profile& prof);

  /**
   * @brief Get statistics of a profile.
   *
   * @param name The name of the profile.
   * @param result Statistics to be returned.
   *
   * @return This function returns 0 if this class has the statistics of 'name'.
   * Otherwise, this function returns non-zero value.
   */
  int get(const std::string& name, SingleProfileStatistics& result) const;
  void getAll(std::map<std::string, SingleProfileStatistics>& results) const;

  void clear();
};

} // namespace mtkw

#endif
