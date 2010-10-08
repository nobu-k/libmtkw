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

class ProfilerMessenger : noncopyable {
private:
  std::ostringstream oss;

public:
  ~ProfilerMessenger() {
    Manager& mgr = Manager::instance();
    if (mgr.isEnabled()) mgr.appendMessage(oss.str());
  }

  template<typename T>
  ProfilerMessenger& operator <<(const T& v) {
    oss << v;
    return *this;
  }
};

} // namespace detail

int initialize(bool enabled = false);
inline int enable(bool e = true) {
  return Manager::instance().enable(e);
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
#define MTKW_PROFILE_N(name) \
  if (::mtkw::detail::ScopedProfiler libmtkw_scoped_profiler_long_name_dayo_ = ::mtkw::detail::ScopedProfiler(name)) {} else 

#define MTKW_PROFILE() MTKW_PROFILE_N(MTKW_CURRENT_FUNCTION)

#define MTKW_SCOPED_PROFILE_N(name) \
  ::mtkw::detail::ScopedProfiler libmtkw_scoped_profiler_long_name_dayo_ = ::mtkw::detail::ScopedProfiler(name)

#define MTKW_SCOPED_PROFILE() MTKW_SCOPED_PROFILE_N(MTKW_CURRENT_FUNCTION)

#define MTKW_MESSAGE() ::mtkw::detail::ProfilerMessenger()

} // namespace mtkw

#endif
