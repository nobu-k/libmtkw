#include "time.hpp"

#include <time.h>
#include <sys/time.h>

namespace mtkw {
namespace {
double gettimeofday_sec() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + tv.tv_usec * 1e-6;
}
} // namespace

double currentTime() {
  // TODO: support other OS
  return gettimeofday_sec();
}

} // namespace mtkw
