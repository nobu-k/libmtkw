#include <gtest/gtest.h>

#include "profiler.hpp"

namespace mtkw {

TEST(DefaultInitializationTest, default_initialization_test) {
  // just check if this code dies
  MTKW_PROFILE() {
  }
}

} // namespace mtkw
