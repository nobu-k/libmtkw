#include <gtest/gtest.h>

#include "profiler.hpp"

#ifdef STATISTICAL_PROFILER_TEST
#define ProfilerTest StatisticalProfilerTest
#define MTKW_APROFILE_N MTKW_SPROFILE_N
#define MTKW_APROFILE MTKW_SPROFILE
#define MTKW_SCOPED_APROFILE_N MTKW_SCOPED_SPROFILE_N
#define MTKW_SCOPED_APROFILE MTKW_SCOPED_SPROFILE
#else // normal profiler
#define ProfilerTest NormalProfilerTest
#define MTKW_APROFILE_N MTKW_PROFILE_N
#define MTKW_APROFILE MTKW_PROFILE
#define MTKW_SCOPED_APROFILE_N MTKW_SCOPED_PROFILE_N
#define MTKW_SCOPED_APROFILE MTKW_SCOPED_PROFILE
#endif

namespace mtkw {

TEST(ProfilerTest, simple_profile) {
  ASSERT_EQ(0, enable());
  MTKW_APROFILE_N("P1") {
    ASSERT_FALSE(!getCurrentProfile());
    MTKW_DLOG() << "Red Bull";
    MTKW_DLOG() << " oisiidesu(^q^)";

    MTKW_APROFILE_N("P2") {
      ASSERT_FALSE(!getCurrentProfile());
      MTKW_DLOG() << "hogehoge" << 1 << 2 << 3;
    }
    ASSERT_FALSE(!getCurrentProfile());
  }
  ASSERT_TRUE(!getCurrentProfile());

  ProfilePtr p1 = getLastProfile();
  ASSERT_FALSE(!p1);

  EXPECT_EQ("P1", p1->name);
  EXPECT_EQ("Red Bull oisiidesu(^q^)", p1->debug_log);
  ASSERT_EQ(1, p1->subprofiles.size());

  ProfilePtr p2 = p1->subprofiles[0];
  ASSERT_FALSE(!p2);
  EXPECT_EQ("P2", p2->name);
  EXPECT_EQ("hogehoge123", p2->debug_log);
  EXPECT_TRUE(p2->subprofiles.empty());

#ifdef STATISTICAL_PROFILER_TEST
  ProfileStatistics st;
  st.addAll(*p1);
  ASSERT_EQ(2, st.size());

  SingleProfileStatistics s;
  ASSERT_EQ(0, st.get("P1", s));
  EXPECT_EQ(1, s.called());

  ASSERT_EQ(0, st.get("P2", s));
  EXPECT_EQ(1, s.called());
#endif
}

namespace funprof {
void f() {
  MTKW_SCOPED_APROFILE();
}
} // namespace funprof

TEST(ProfilerTest, simple_function_profile) {
  ASSERT_EQ(0, enable());
  MTKW_APROFILE() {
    funprof::f();
  }

  ProfilePtr p = getLastProfile();
  EXPECT_FALSE(!p);
  EXPECT_FALSE(p->name.empty());
  ASSERT_EQ(1, p->subprofiles.size());

  ProfilePtr f = p->subprofiles[0];
  EXPECT_FALSE(!f);
  EXPECT_FALSE(f->name.empty());
}

namespace nested {
void f() {
  MTKW_SCOPED_APROFILE();
  MTKW_DLOG() << "hoge";
}

void g() {
  MTKW_APROFILE_N("g") {
    MTKW_DLOG() << "Red";
    f();
    MTKW_DLOG() << " Bull";
  }
}

void h() {
  MTKW_SCOPED_APROFILE_N("h");
  MTKW_DLOG() << 1 << " " << 2;
  g();
  MTKW_DLOG() << " " << 3;
}
} // namespace nested

TEST(ProfilerTest, nested) {
  ASSERT_EQ(0, enable());
  MTKW_APROFILE() {
    nested::h();
  }

  ProfilePtr p = getLastProfile();
  ASSERT_FALSE(!p);
  ASSERT_EQ(1, p->subprofiles.size());

  ProfilePtr h = p->subprofiles[0];
  ASSERT_FALSE(!h);
  EXPECT_EQ("h", h->name);
  EXPECT_EQ("1 2 3", h->debug_log);
  EXPECT_EQ(1, h->subprofiles.size());

  ProfilePtr g = h->subprofiles[0];
  ASSERT_FALSE(!g);
  EXPECT_EQ("g", g->name);
  EXPECT_EQ("Red Bull", g->debug_log);
  ASSERT_EQ(1, g->subprofiles.size());

  ProfilePtr f = g->subprofiles[0];
  ASSERT_FALSE(!f);
  EXPECT_FALSE(f->name.empty());
  EXPECT_EQ("hoge", f->debug_log);
  EXPECT_TRUE(f->subprofiles.empty());
}

namespace multi {
void f() {
  MTKW_APROFILE_N("f1") MTKW_DLOG() << "Red";
  MTKW_APROFILE_N("f2") MTKW_DLOG() << "Bull";
}

void g() {
  MTKW_APROFILE() MTKW_DLOG() << "oisiidesu";
}

void h() {
  MTKW_APROFILE() {
    MTKW_DLOG() << "(";
    g();
    MTKW_DLOG() << "^q^";
    f();
    MTKW_DLOG() << ")";
  }
}

void i() {
  MTKW_APROFILE() MTKW_DLOG() << "hoge";
}
} // namespace multi

TEST(ProfilerTest, multi_subprofile) {
  ASSERT_EQ(0, enable());
  MTKW_APROFILE() {
    multi::i();
    multi::h();
  }

  ProfilePtr p = getLastProfile();
  ASSERT_FALSE(!p);
  ASSERT_EQ(2, p->subprofiles.size());

  ProfilePtr i = p->subprofiles[0];
  ASSERT_FALSE(!i);
  EXPECT_EQ("hoge", i->debug_log);
  EXPECT_TRUE(i->subprofiles.empty());

  ProfilePtr h = p->subprofiles[1];
  ASSERT_FALSE(!h);
  EXPECT_EQ("(^q^)", h->debug_log);
  ASSERT_EQ(3, h->subprofiles.size());

  ProfilePtr g = h->subprofiles[0];
  ASSERT_FALSE(!g);
  EXPECT_EQ("oisiidesu", g->debug_log);
  EXPECT_TRUE(g->subprofiles.empty());

  ProfilePtr f1 = h->subprofiles[1];
  ASSERT_FALSE(!f1);
  EXPECT_EQ("f1", f1->name);
  EXPECT_EQ("Red", f1->debug_log);
  EXPECT_TRUE(f1->subprofiles.empty());

  ProfilePtr f2 = h->subprofiles[2];
  ASSERT_FALSE(!f2);
  EXPECT_EQ("f2", f2->name);
  EXPECT_EQ("Bull", f2->debug_log);
  EXPECT_TRUE(f2->subprofiles.empty());
}

} // namespace mtkw

int main(int argc, char* argv[]) {
  if (mtkw::initialize() != 0) {
    LOG(ERROR) << "Failed to initialize Manager.";
    return -1;
  }

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
