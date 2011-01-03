#ifndef LIBMTKW_SRC_MANAGER_HPP_
#define LIBMTKW_SRC_MANAGER_HPP_

#include <cassert>
#include "util.hpp"
#include "profile.hpp"
#include "flags.hpp"

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
  static int initialize(ThreadLocalStorage* storage,
                        const Flags& default_flags);

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
  int enable(bool e = true, bool debug_mode = true);
  int disable();
  int setFlags(const Flags& flags);
  void setDefaultFlags(const Flags& default_flags);
  bool isEnabled() const;
  bool isDebugMode() const;
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

  // TODO: Is simply returing const ProfileStatistics& is better?
  /**
   * @name Statistics functions
   */
  //@{
  void addStatistics(const ProfilePtr& p);
  void clearStatistics();

  int getStatistics(const std::string& name, SingleProfileStatistics& result) const;
  void getAllStatistics(std::map<std::string, SingleProfileStatistics>& results) const;
  //@}
};

} // namespace mtkw

#endif
