#ifndef LIBMTKW_SRC_THREAD_HPP_
#define LIBMTKW_SRC_THREAD_HPP_

#ifdef MTKW_PFI_INTERNAL

#include <pficommon/concurrent/mutex.h>
#include <pficommon/concurrent/rwmutex.h>
#include <pficommon/concurrent/lock.h>

namespace mtkw {
namespace thread {
typedef pfi::concurrent::mutex mutex;
typedef pfi::concurrent::rw_mutex rw_mutex;
typedef pfi::concurrent::scoped_lock lock;
typedef pfi::concurrent::scoped_rlock rlock;
typedef pfi::concurrent::scoped_wlock wlock;
} // namespace thread
} // namespace mtkw

#else
// TODO: prepare for other users
// 1. boost
// 2. pthread
#endif

#endif // LIBMTKW_SRC_THREAD_HPP_
