#include "profiler.hpp"

#include <cstdlib>
#include "thread_local_storage.hpp"
#include "flags.hpp"
#include "env.hpp"

namespace mtkw {

int initialize() {
  Flags flags;
  flags.enabled = false;
  flags.debug_mode = true;
  env::getFlags(flags);
  return initialize(flags);
}

int initialize(bool enabled, bool debug_mode) {
  Flags flags;
  flags.enabled = false;
  flags.debug_mode = false;
  return initialize(flags);
}

int initialize(const Flags& flags) {
  return Manager::initialize(createDefaultThreadLocalStorage(), flags);
}

} // namespace mtkw
