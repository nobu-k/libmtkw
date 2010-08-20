#ifndef LIBMTKW_SRC_THREAD_LOCAL_STORAGE_HPP_
#define LIBMTKW_SRC_THREAD_LOCAL_STORAGE_HPP_

namespace mtkw {

class ThreadLocalManager;

class ThreadLocalStorage {
public:
  virtual ~ThreadLocalStorage() {}
  virtual ThreadLocalManager* get() = 0;
};

class ThreadLocalStorageFactory {
public:
  virtual ~ThreadLocalStorageFactory() {}
  virtual ThreadLocalStorage* create() = 0;
};

} // namespace mtkw

#endif
