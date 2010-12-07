#include "profiler.hpp"

#include "thread_local_storage.hpp"

namespace mtkw {

int initialize(bool enabled, bool debug_mode) {
  if (Manager::initialize(createDefaultThreadLocalStorage()) != 0) {
    return -1;
  }
  return enable(enabled, debug_mode);
}

} // namespace mtkw
