#include "env.hpp"

#include <cstdlib>

namespace mtkw {
namespace env {

bool checkTrue(const std::string& v) {
  #ifdef MTKW_PFI_INTERNAL
  // check hyper mtkw mode
  if (v == "hai" || v == "Hai" || v == "HAI") return true;
#endif
  return v == "yes" || v == "true" || v == "t" || v == "1";
}

int getBoolean(const std::string& key, bool& res) {
  const char* val = std::getenv(key.c_str());
  if (!val) return -1;

  const std::string v = val;
  if (v.empty()) return -1;
  res = checkTrue(v);
  return 0;
}

void getFlags(bool& enabled, bool& debug_mode) {
  getEnabledFlag(enabled);
  getDebugFlag(debug_mode);
}

int getEnabledFlag(bool& enabled) {
  if (getBoolean("MTKW", enabled) != 0) {
    return getBoolean("MTKW_ENABLE", enabled);
  }
  return 0;
}

void getEnabledFlag(bool& enabled, bool default_value) {
  if (getEnabledFlag(enabled) != 0) {
    enabled = default_value;
  }
}

int getDebugFlag(bool& debug_mode) {
  return getBoolean("MTKW_DEBUG", debug_mode);
}

void getDebugFlag(bool& debug_mode, bool default_value) {
  if (getDebugFlag(debug_mode) != 0) {
    debug_mode = default_value;
  }
}

} // namespace env
} // namespace mtkw
