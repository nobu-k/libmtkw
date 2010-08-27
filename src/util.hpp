#ifndef LIBMTKW_SRC_UTIL_HPP_
#define LIBMTKW_SRC_UTIL_HPP_

// TODO: support Visual C++
#ifdef MTKW_PFI_INTERNAL
#include <pficommon/lang/shared_ptr.h>
#include <pficommon/lang/noncopyable.h>
#else
#include <tr1/memory>
// TODO: include noncopyable
#endif

#if defined MTKW_PFI_INTERNAL || defined MTKW_WITH_GLOG
#include <glog/logging.h>
#else
// TODO: define LOGs with null_ostream like class
#endif

namespace mtkw {

#ifdef MTKW_PFI_INTERNAL
using pfi::lang::shared_ptr;
typedef pfi::lang::noncopyable noncopyable;
#define MTKW_CURRENT_FUNCTION __PRETTY_FUNCTION__
#else
using std::tr1::shared_ptr;
// TODO: use noncopyable
#define MTKW_CURRENT_FUNCTION "(unknown)"
#endif

// TODO: define unordered_map

} // namespace mtkw

#endif
