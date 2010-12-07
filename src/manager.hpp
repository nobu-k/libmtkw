#ifndef LIBMTKW_SRC_MANAGER_HPP_
#define LIBMTKW_SRC_MANAGER_HPP_

#include <cassert>
#include "util.hpp"
#include "profile.hpp"

namespace mtkw {

class ThreadLocalStorage;
class ThreadLocalManager;

class Manager {
private:
  Manager();
  static Manager* _instance;
  shared_ptr<ThreadLocalStorage> _tls;
  ProfileStatistics _statistics;

private:
  static int uninitializedCreate();
  ThreadLocalManager* getTlsMgr() const;

public:
  ~Manager();

  /**
   * @note Call this function before calling instance().
   * @note ThreadLocalStorage* will automatically be deleted.
   */
  static int initialize(ThreadLocalStorage* storage);

  /**
   * @note This function is only for test codes.
   * @note Calling this function while profiling something is very dangerous.
   */
  static int resetThreadLocalStorage(ThreadLocalStorage* storage);

  static Manager& instance() {
    if (!_instance) {
      int res = uninitializedCreate();
      assert(res == 0 && "Initialization failed.");
    }
    return *_instance;
  }

  /**
   * @name Profiling functions
   */
  //@{
  int enable(bool e = true);
  int disable();
  bool isEnabled() const;
  int beginProfile(const std::string& name, bool gen_stat = false);
  int endProfile();
  int appendProfile(const ProfilePtr& p);
  int setDebugLog(const std::string& msg);
  int appendDebugLog(const std::string& msg);
  int setInformation(const std::string& key, const std::string& value);

  /**
   * @note For boolean
   */
  int setInformation(const std::string& key, bool value) {
    // FIXME: remove redundant allocation
    return setInformation(key, value ? "true" : "false");
  }

  /**
   * @note For automatic conversion.
   */
  template<typename T>
  int setInformation(const std::string& key,
                     const T& value) {
    return setInformation(key, lexical_cast<std::string>(value));
  }
  ProfilePtr getCurrentProfile() const;
  ProfilePtr getLastProfile() const;
  //@}

  /**
   * @name Statistics functions
   */
  //@{
  void addStatistics(const ProfilePtr& p);
  void clearStatistics();
  // TODO: add function to generate statistics for a single profile
  //@}
};

} // namespace mtkw

#endif
