#include "thread_local_storage.hpp"

#include "pthread_local_storage.hpp"
#include "util.hpp"

namespace mtkw {

ThreadLocalStorage* createDefaultThreadLocalStorage() {
  // TODO: support other ThreadLocalStorage
  PthreadLocalStorage* tls = new PthreadLocalStorage();
  if (tls->initialize() != 0) {
    LOG(ERROR) << "Failed to initialize PthreadLocalStorage.";
    return NULL;
  }
  return tls;
}

} // namespace mtkw
