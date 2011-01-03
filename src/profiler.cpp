#include "profiler.hpp"

#include <cstdlib>
#include "thread_local_storage.hpp"
#include "env.hpp"

namespace mtkw {

int initialize() {
  bool enabled = false;
  bool debug_mode = true;
  env::getFlags(enabled, debug_mode);
  return initialize(enabled, debug_mode);
}

int initialize(bool enabled, bool debug_mode) {
  if (Manager::initialize(createDefaultThreadLocalStorage()) != 0) {
    return -1;
  }
  return enable(enabled, debug_mode);
}

} // namespace mtkw
