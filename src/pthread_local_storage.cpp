#include "pthread_local_storage.hpp"

#include <pthread.h>
#include "thread_local_manager.hpp"

namespace mtkw {

struct PthreadLocalStorage::Pimpl {
  pthread_key_t tls_key;
};

PthreadLocalStorage::PthreadLocalStorage() : _pimpl(NULL) {
}

PthreadLocalStorage::~PthreadLocalStorage() {
  if (_pimpl) {
    pthread_key_delete(_pimpl->tls_key);
    delete _pimpl;
  }
}

ThreadLocalManager* PthreadLocalStorage::get() {
  if (!_pimpl) {
    LOG(ERROR) << "PthreadLocalStorage is not initialized yet.";
    return NULL;
  }

  // get thread local one
  ThreadLocalManager* ptr = pthread_getspecific(_pimpl->tls_key);
  if (ptr == NULL) {
    ptr = new ThreadLocalManager();
    if (pthread_setspecific(_pimpl->tls_key, ptr) != 0) {
      delete ptr;
      LOG(ERROR) << "pthread_setspecific failed";
      return NULL;
    }
  }
  return ptr;
}

namespace {
void tlsDestructor(void* ptr) {
  ThreadLocalManager* mgr = static_cast<ThreadLocalManager*>(ptr);
  delete mgr;
}
} // namespace

int PthreadLocalStorage::initialize() {
  if (_pimpl != NULL) {
    LOG(ERROR) << "PthreadLocalStorage is already initialized.";
    return -1;
  }
  _pimpl = new Pimpl();
  if (pthread_key_create(&_pimpl->tls_key, &tlsDestructor) != 0) {
    delete _pimpl;
    _pimpl = NULL;
    LOG(ERROR) << "pthread_key_create failed.";
    return -1;
  }
  return 0;
}

} // namespace mtkw
