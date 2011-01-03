#include <gtest/gtest.h>

#include <cstdlib>
#include "env.hpp"

// 0: the key to be set (MTKW or MTKW_ENABLE)
// 1: a value for 0
// 2: a value for MTKW_DEBUG
// 3: a default value for enabled
// 4: a default value for disabled
typedef std::tr1::tuple<const char*, const char*, const char*, bool, bool> DataType;

namespace mtkw {
namespace env {
namespace {
const std::vector<const char*>& EnvKeys() {
  static bool initialized = false;
  static std::vector<const char*> v;
  if (initialized) return v;
  initialized = true;

  v.push_back("MTKW");
  v.push_back("MTKW_ENABLE");
  return v;
}

const std::vector<const char*>& YesValues() {
  static bool initialized = false;
  static std::vector<const char*> v;
  if (initialized) return v;
  initialized = true;

  v.push_back("yes");
  v.push_back("true");
  v.push_back("t");
  v.push_back("1");

#ifdef MTKW_PFI_INTERNAL
  v.push_back("hai");
  v.push_back("Hai");
  v.push_back("HAI");
#endif
  return v;
}

const std::vector<const char*>& NoValues() {
  static bool initialized = false;
  static std::vector<const char*> v;
  if (initialized) return v;
  initialized = true;

  v.push_back("ye");
  v.push_back("y");
  v.push_back("no");
  v.push_back("false");
  v.push_back("f");
  v.push_back("11");
  v.push_back("0");
  v.push_back("what ever");
  return v;
}

void SetEnv(const DataType& d) {
  const char* mtkw_key = std::tr1::get<0>(d);
  const char* mtkw_value = std::tr1::get<1>(d);
  const char* debug_value = std::tr1::get<2>(d);
  setenv(mtkw_key, mtkw_value, 1);
  setenv("MTKW_DEBUG", debug_value, 1);
  if (std::tr1::get<0>(d) == std::string("MTKW")) {
    unsetenv("MTKW_ENABLE");
  } else {
    unsetenv("MTKW");
  }
}
} // namespace

// TODO: write test with no env key

class EnvEnabledTest : public testing::TestWithParam<DataType> {
protected:
  virtual void SetUp() {
    SetEnv(GetParam());
    mtkw_enabled = std::tr1::get<3>(GetParam());
    mtkw_debug = std::tr1::get<4>(GetParam());
  }

  bool mtkw_enabled;
  bool mtkw_debug;
};

class EnvEnabledDebugTrueTest : public EnvEnabledTest {
};

INSTANTIATE_TEST_CASE_P(EnvEnabledDebugTrueTestInstance,
                        EnvEnabledDebugTrueTest,
                        testing::Combine(testing::ValuesIn(EnvKeys()),
                                         testing::ValuesIn(YesValues()),
                                         testing::ValuesIn(YesValues()),
                                         testing::Bool(), testing::Bool()));

TEST_P(EnvEnabledDebugTrueTest, get_flags_test) {
  getFlags(mtkw_enabled, mtkw_debug);
  EXPECT_TRUE(mtkw_enabled);
  EXPECT_TRUE(mtkw_debug);
}

TEST_P(EnvEnabledDebugTrueTest, get_enabled_flag_test) {
  ASSERT_EQ(0, getEnabledFlag(mtkw_enabled));
  EXPECT_TRUE(mtkw_enabled);
}

TEST_P(EnvEnabledDebugTrueTest, get_debug_flag_test) {
  ASSERT_EQ(0, getDebugFlag(mtkw_debug));
  EXPECT_TRUE(mtkw_debug);
}

class EnvEnabledDebugFalseTest : public EnvEnabledTest {
};

INSTANTIATE_TEST_CASE_P(EnvEnabledDebugFalseTestInstance,
                        EnvEnabledDebugFalseTest,
                        testing::Combine(testing::ValuesIn(EnvKeys()),
                                         testing::ValuesIn(YesValues()),
                                         testing::ValuesIn(NoValues()),
                                         testing::Bool(), testing::Bool()));

TEST_P(EnvEnabledDebugFalseTest, get_flags_test) {
  getFlags(mtkw_enabled, mtkw_debug);
  EXPECT_TRUE(mtkw_enabled);
  EXPECT_FALSE(mtkw_debug);
}

TEST_P(EnvEnabledDebugFalseTest, get_enabled_flag_test) {
  ASSERT_EQ(0, getEnabledFlag(mtkw_enabled));
  EXPECT_TRUE(mtkw_enabled);
}

TEST_P(EnvEnabledDebugFalseTest, get_debug_flag_test) {
  ASSERT_EQ(0, getDebugFlag(mtkw_debug));
  EXPECT_FALSE(mtkw_debug);
}

class EnvDisabledDebugTrueTest : public EnvEnabledTest {
};

INSTANTIATE_TEST_CASE_P(EnvDisabledDebugTrueTestInstance,
                        EnvDisabledDebugTrueTest,
                        testing::Combine(testing::ValuesIn(EnvKeys()),
                                         testing::ValuesIn(NoValues()),
                                         testing::ValuesIn(YesValues()),
                                         testing::Bool(), testing::Bool()));

TEST_P(EnvDisabledDebugTrueTest, get_flags_test) {
  getFlags(mtkw_enabled, mtkw_debug);
  EXPECT_FALSE(mtkw_enabled);
  EXPECT_TRUE(mtkw_debug);
}

TEST_P(EnvDisabledDebugTrueTest, get_enabled_flag_test) {
  ASSERT_EQ(0, getEnabledFlag(mtkw_enabled));
  EXPECT_FALSE(mtkw_enabled);
}

TEST_P(EnvDisabledDebugTrueTest, get_debug_flag_test) {
  ASSERT_EQ(0, getDebugFlag(mtkw_debug));
  EXPECT_TRUE(mtkw_debug);
}

class EnvDisabledDebugFalseTest : public EnvEnabledTest {
};

INSTANTIATE_TEST_CASE_P(EnvDisabledDebugFalseTestInstance,
                        EnvDisabledDebugFalseTest,
                        testing::Combine(testing::ValuesIn(EnvKeys()),
                                         testing::ValuesIn(NoValues()),
                                         testing::ValuesIn(NoValues()),
                                         testing::Bool(), testing::Bool()));

TEST_P(EnvDisabledDebugFalseTest, get_flags_test) {
  getFlags(mtkw_enabled, mtkw_debug);
  EXPECT_FALSE(mtkw_enabled);
  EXPECT_FALSE(mtkw_debug);
}

TEST_P(EnvDisabledDebugFalseTest, get_enabled_flag_test) {
  ASSERT_EQ(0, getEnabledFlag(mtkw_enabled));
  EXPECT_FALSE(mtkw_enabled);
}

TEST_P(EnvDisabledDebugFalseTest, get_debug_flag_test) {
  ASSERT_EQ(0, getDebugFlag(mtkw_debug));
  EXPECT_FALSE(mtkw_debug);
}

} // namespace env
} // namespace mtkw
