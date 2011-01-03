#ifndef LIBMTKW_SRC_THREAD_LOCAL_STORAGE_HPP_
#define LIBMTKW_SRC_THREAD_LOCAL_STORAGE_HPP_

namespace mtkw {

class ThreadLocalManager;
struct Flags;

class ThreadLocalStorage {
public:
  virtual ~ThreadLocalStorage() {}

  /**
   * @note This function must be thread-safe.
   */
  virtual void setDefaultFlags(const Flags& default_flags) = 0;
  virtual ThreadLocalManager* get() = 0;
};

ThreadLocalStorage* createDefaultThreadLocalStorage();

} // namespace mtkw

#endif
