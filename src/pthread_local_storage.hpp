#ifndef LIBMTKW_SRC_PTHREAD_LOCAL_STORAGE_HPP_
#define LIBMTKW_SRC_PTHREAD_LOCAL_STORAGE_HPP_

#include "thread_local_storage.hpp"
#include "util.hpp"

namespace mtkw {

class PthreadLocalStorage : public ThreadLocalStorage, private noncopyable {
private:
  struct Pimpl;
  Pimpl* _pimpl;

public:
  PthreadLocalStorage();
  virtual ~PthreadLocalStorage();

  virtual ThreadLocalManager* get() = 0;

  /**
   * @note This function is neccessary to avoid using exception.
   * @note Call this function before creating any threads.
   */
  int initialize();
};

} // namespace mtkw

#endif
