#include <gtest/gtest.h>

#include "profiler.hpp"

namespace mtkw {

TEST(ProfilerTest, simple_profile) {
  ASSERT_EQ(0, enable());
  MTKW_PROFILE_N("P1") {
    ASSERT_FALSE(!getCurrentProfile());
    ASSERT_TRUE(getCurrentProfile()->isRoot());
    MTKW_MESSAGE() << "Red Bull";
    MTKW_MESSAGE() << " oisiidesu(^q^)";

    MTKW_PROFILE_N("P2") {
      ASSERT_FALSE(!getCurrentProfile());
      ASSERT_FALSE(getCurrentProfile()->isRoot());
      MTKW_MESSAGE() << "hogehoge" << 1 << 2 << 3;
    }
    ASSERT_FALSE(!getCurrentProfile());
  }
  ASSERT_TRUE(!getCurrentProfile());

  ProfilePtr p1 = getLastProfile();
  ASSERT_FALSE(!p1);

  EXPECT_EQ("P1", p1->name);
  EXPECT_EQ("Red Bull oisiidesu(^q^)", p1->message);
  ASSERT_EQ(1, p1->subprofiles[0]);

  ProfilePtr p2 = p1->subprofiles[0];
  ASSERT_FALSE(!p2);
  EXPECT_TRUE(p1 == p2->parent);
  EXPECT_EQ("P2", p2->name);
  EXPECT_EQ("hogehoge123", p2->message);
  EXPECT_TRUE(p2->subprofiles.empty());
}

namespace funprof {
void f() {
  MTKW_SCOPED_PROFILE();
}
} // namespace funprof

TEST(ProfilerTest, simple_function_profile) {
  ASSERT_EQ(0, enable());
  MTKW_PROFILE() {
    funprof::f();
  }

  ProfilePtr p = getLastProfile();
  EXPECT_FALSE(!p);
  EXPECT_FALSE(p->name.empty());
  ASSERT_EQ(1, p->subprofiles.size());

  ProfilePtr f = p->subprofiles[0];
  EXPECT_FALSE(!f);
  EXPECT_TRUE(p == f->parent);
  EXPECT_FALSE(f->name.empty());
}

namespace nested {
void f() {
  MTKW_SCOPED_PROFILE();
  MTKW_MESSAGE() << "hoge";
}

void g() {
  MTKW_PROFILE_N("g") {
    MTKW_MESSAGE() << "Red";
    f();
    MTKW_MESSAGE() << " Bull";
  }
}

void h() {
  MTKW_SCOPED_PROFILE_N("h");
  MTKW_MESSAGE() << 1 << " " << 2;
  g();
  MTKW_MESSAGE() << " " << 3;
}
} // namespace nested

TEST(ProfilerTest, nested) {
  ASSERT_EQ(0, enable());
  MTKW_PROFILE() {
    nested::h();
  }

  ProfilePtr p = getLastProfile();
  ASSERT_FALSE(!p);
  ASSERT_EQ(1, p->subprofiles.size());

  ProfilePtr h = p->subprofiles[0];
  ASSERT_FALSE(!h);
  EXPECT_EQ("h", h->name);
  EXPECT_EQ("1 2 3", h->message);
  EXPECT_EQ(1, h->subprofiles.size());

  ProfilePtr g = h->subprofiles[0];
  ASSERT_FALSE(!g);
  EXPECT_TRUE(h == g->parent);
  EXPECT_EQ("g", g->name);
  EXPECT_EQ("Red Bull", g->message);
  ASSERT_EQ(1, g->subprofiles.size());

  ProfilePtr f = g->subprofiles[0];
  ASSERT_FALSE(!f);
  EXPECT_TRUE(g == f->parent);
  EXPECT_FALSE(f->name.empty());
  EXPECT_EQ("hoge", f->message);
  EXPECT_TRUE(f->subprofiles.empty());
}

namespace multi {
void f() {
  MTKW_PROFILE_N("f1") MTKW_MESSAGE() << "Red";
  MTKW_PROFILE_N("f2") MTKW_MESSAGE() << "Bull";
}

void g() {
  MTKW_PROFILE() MTKW_MESSAGE() << "oisiidesu";
}

void h() {
  MTKW_PROFILE() {
    MTKW_MESSAGE() << "(";
    g();
    MTKW_MESSAGE() << "^q^";
    f();
    MTKW_MESSAGE() << ")";
  }
}

void i() {
  MTKW_PROFILE() MTKW_MESSAGE() << "hoge";
}
} // namespace multi

TEST(ProfilerTest, multi_subprofile) {
  ASSERT_EQ(0, enable());
  MTKW_PROFILE() {
    multi::i();
    multi::h();
  }

  ProfilePtr p = getLastProfile();
  ASSERT_FALSE(!p);
  ASSERT_EQ(2, p->subprofiles.size());

  ProfilePtr i = p->subprofiles[0];
  ASSERT_FALSE(!i);
  EXPECT_EQ("hoge", i->message);
  EXPECT_TRUE(i->subprofiles.empty());

  ProfilePtr h = p->subprofiles[1];
  ASSERT_FALSE(!h);
  EXPECT_EQ("(^q^)", h->message);
  ASSERT_EQ(3, h->subprofiles.size());

  ProfilePtr g = h->subprofiles[0];
  ASSERT_FALSE(!g);
  EXPECT_EQ("oisiidesu", g->message);
  EXPECT_TRUE(g->subprofiles.empty());

  ProfilePtr f1 = h->subprofiles[1];
  ASSERT_FALSE(!f1);
  EXPECT_EQ("f1", f1->name);
  EXPECT_EQ("Red", f1->message);
  EXPECT_TRUE(f1->subprofiles.empty());

  ProfilePtr f2 = h->subprofiles[2];
  ASSERT_FALSE(!f2);
  EXPECT_EQ("f2", f2->name);
  EXPECT_EQ("Bull", f2->message);
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
