#ifndef LIBMTKW_SRC_PROFILER_HPP_
#define LIBMTKW_SRC_PROFILER_HPP_

#include <sstream>
#include "profile.hpp"
#include "manager.hpp"
#include "util.hpp"

namespace mtkw {
namespace detail {

class ScopedProfiler {
private:
  typedef void (ScopedProfiler::*false_bool_type)() const;

public:
  explicit ScopedProfiler(const char* name, bool gen_stat = false) {
    Manager& mgr = Manager::instance();
    if (mgr.isEnabled()) {
      Manager::instance().beginProfile(name, gen_stat);
    }
  }
  explicit ScopedProfiler(const std::string& name, bool gen_stat = false) {
    Manager& mgr = Manager::instance();
    if (mgr.isEnabled()) mgr.beginProfile(name, gen_stat);
  }

  ~ScopedProfiler() {
    Manager& mgr = Manager::instance();
    if (mgr.isEnabled()) mgr.endProfile();
  }

  operator false_bool_type() const { return 0; }
};

class ProfilerDebugLogger : noncopyable {
private:
  std::ostringstream oss;

public:
  ~ProfilerDebugLogger() {
    Manager& mgr = Manager::instance();
    if (mgr.isEnabled()) mgr.appendDebugLog(oss.str());
  }

  template<typename T>
  ProfilerDebugLogger& operator <<(const T& v) {
    oss << v;
    return *this;
  }
};

} // namespace detail

int initialize(bool enabled = false, bool debug_mode = true);
inline int enable(bool e = true, bool debug_mode = true) {
  return Manager::instance().enable(e, debug_mode);
}

inline int disable() {
  return Manager::instance().disable();
}

inline ProfilePtr getCurrentProfile() {
  return Manager::instance().getCurrentProfile();
}

inline ProfilePtr getLastProfile() {
  return Manager::instance().getLastProfile();
}

// Macros to support profiling
#define MTKW_PROFILE_INTERNAL_(name, gen_stat) \
  if (::mtkw::detail::ScopedProfiler libmtkw_scoped_profiler_long_name_dayo_ = ::mtkw::detail::ScopedProfiler(name, gen_stat)) {} else

#define MTKW_SCOPED_PROFILE_INTERNAL_(name, gen_stat) \
  ::mtkw::detail::ScopedProfiler libmtkw_scoped_profiler_long_name_dayo_ = ::mtkw::detail::ScopedProfiler(name, gen_stat)

#define MTKW_PROFILE_N(name) MTKW_PROFILE_INTERNAL_(name, false)
#define MTKW_PROFILE() MTKW_PROFILE_N(MTKW_CURRENT_FUNCTION)

#define MTKW_SCOPED_PROFILE_N(name) MTKW_SCOPED_PROFILE_INTERNAL_(name, false)
#define MTKW_SCOPED_PROFILE() MTKW_SCOPED_PROFILE_N(MTKW_CURRENT_FUNCTION)

// statistical version
#define MTKW_SPROFILE_N(name) MTKW_PROFILE_INTERNAL_(name, true)
#define MTKW_SPROFILE() MTKW_SPROFILE_N(MTKW_CURRENT_FUNCTION)

#define MTKW_SCOPED_SPROFILE_N(name) MTKW_SCOPED_PROFILE_INTERNAL_(name, true)
#define MTKW_SCOPED_SPROFILE() MTKW_SCOPED_SPROFILE_N(MTKW_CURRENT_FUNCTION)

// for functions
#define MTKW_PROFILE_FUNCTION() MTKW_SCOPED_SPROFILE()

// information generation
#define MTKW_INFO(key, value) \
  ::mtkw::Manager::instance().setInformation(key, value);

// debug utilities
#define MTKW_DLOG() ::mtkw::detail::ProfilerDebugLogger()
#define MTKW_DEBUG() \
  if (!::mtkw::Manager::instance().isDebugMode()) {} else

} // namespace mtkw

#endif
