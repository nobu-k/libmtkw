#ifndef LIBMTKW_SRC_THREAD_LOCAL_STORAGE_HPP_
#define LIBMTKW_SRC_THREAD_LOCAL_STORAGE_HPP_

namespace mtkw {

class ThreadLocalManager;

class ThreadLocalStorage {
public:
  virtual ~ThreadLocalStorage() {}
  virtual ThreadLocalManager* get() = 0;
};

ThreadLocalStorage* createDefaultThreadLocalStorage();

} // namespace mtkw

#endif
