#include "profiler.hpp"

#include "thread_local_storage.hpp"

namespace mtkw {

int initialize() {
  return Manager::initialize(createDefaultThreadLocalStorage());
}

} // namespace mtkw
