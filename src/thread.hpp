#ifndef LIBMTKW_SRC_THREAD_HPP_
#define LIBMTKW_SRC_THREAD_HPP_

#include "util.hpp"

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

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/locks.hpp>

namespace mtkw {
namespace thread {

typedef boost::mutex mutex;
typedef boost::shared_mutex rw_mutex;

class lock : noncopyable {
public:
  lock(mutex& m) : _lk(m) {}

private:
  boost::lock_guard<mutex> _lk;
};

class rlock : noncopyable {
public:
  rlock(rw_mutex& m) : _lk(m) {}

private:
  boost::shared_lock<rw_mutex> _lk;
};

class wlock : noncopyable {
public:
  wlock(rw_mutex& m) : _lk(m) {}

private:
  boost::unique_lock<rw_mutex> _lk;
};

} // namespace thread
} // namespace mtkw
#endif

#endif // LIBMTKW_SRC_THREAD_HPP_
