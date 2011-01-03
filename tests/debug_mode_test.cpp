#include <gtest/gtest.h>

#include "profiler.hpp"

namespace mtkw {

TEST(DebugModeTest, default_configuration) {
  bool debug = false;
  enable(true, true);
  ASSERT_TRUE(isDebugMode());

  MTKW_DEBUG() {
    debug = true;
  }
  ASSERT_TRUE(debug);
}

TEST(DebugModeTest, debug_mode) {
  bool debug = false;
  enable(true, true);
  ASSERT_TRUE(isDebugMode());

  MTKW_DEBUG() {
    debug = true;
  }
  ASSERT_TRUE(debug);
}

TEST(DebugModeTest, non_debug_mode) {
  bool debug = false;
  enable(true, false);
  ASSERT_FALSE(isDebugMode());

  MTKW_DEBUG() {
    debug = true;
  }
  ASSERT_FALSE(debug);
}

TEST(DebugModeTest, disabled_debug_mode) {
  bool debug = false;
  enable(false, true);
  ASSERT_FALSE(isDebugMode());

  MTKW_DEBUG() {
    debug = true;
  }
  ASSERT_FALSE(debug);
}

TEST(DebugModeTest, disabled_all) {
  bool debug = false;
  enable(false, false);
  ASSERT_FALSE(isDebugMode());

  MTKW_DEBUG() {
    debug = true;
  }
  ASSERT_FALSE(debug);
}

TEST(DebugModeTest, disable_after_enable) {
  bool debug = false;
  enable(true, true);
  ASSERT_TRUE(isDebugMode());
  disable();
  ASSERT_FALSE(isDebugMode());

  MTKW_DEBUG() {
    debug = true;
  }
  ASSERT_FALSE(debug);
}

} // namespace mtkw
