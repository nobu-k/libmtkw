#ifndef LIBMTKW_SRC_UTIL_HPP_
#define LIBMTKW_SRC_UTIL_HPP_

// TODO: support Visual C++
#ifdef MTKW_PFI_INTERNAL
#include <pficommon/lang/shared_ptr.h>
#include <pficommon/lang/noncopyable.h>
#include <pficommon/lang/cast.h>
#else
#include <tr1/memory>
#include <boost/utility.hpp>
#include <boost/current_function.hpp>
#include <boost/lexical_cast.hpp>
#endif

#if defined MTKW_PFI_INTERNAL || defined MTKW_WITH_GLOG
#include <glog/logging.h>
#else
// disable glog
class NullStream {
public:
  template<typename T>
  NullStream& operator <<(const T& v) { return *this; }
};
#define LOG(s) NullStream()
#endif

namespace mtkw {

#ifdef MTKW_PFI_INTERNAL
using pfi::lang::shared_ptr;
typedef pfi::lang::noncopyable noncopyable;
using pfi::lang::lexical_cast;
#define MTKW_CURRENT_FUNCTION __PRETTY_FUNCTION__
#else
using std::tr1::shared_ptr;
using boost::noncopyable;
using boost::lexical_cast;
#define MTKW_CURRENT_FUNCTION BOOST_CURRENT_FUNCTION
#endif

// TODO: define unordered_map

} // namespace mtkw

#endif
