#include <gtest/gtest.h>

#include "thread_local_manager.hpp"
#include "time.hpp"

namespace mtkw {

class ThreadLocalManagerTest : public testing::Test {
protected:
  virtual void SetUp() {
    mgr = new ThreadLocalManager();
  }

  virtual void TearDown() {
    delete mgr;
  }

  ThreadLocalManager* mgr;
};

TEST_F(ThreadLocalManagerTest, enable) {
  ASSERT_FALSE(mgr->isEnabled());

  ASSERT_EQ(0, mgr->enable());
  ASSERT_TRUE(mgr->isEnabled());

  ASSERT_EQ(0, mgr->disable());
  ASSERT_FALSE(mgr->isEnabled());

  ASSERT_EQ(0, mgr->enable(true));
  ASSERT_TRUE(mgr->isEnabled());

  ASSERT_EQ(0, mgr->enable(false));
  ASSERT_FALSE(mgr->isEnabled());
}

TEST_F(ThreadLocalManagerTest, enable_while_profiling) {
  // test profile when disabled
  ASSERT_EQ(0, mgr->beginProfile("test1"));
  ASSERT_FALSE(mgr->isEnabled());
  ASSERT_TRUE(!mgr->getCurrentProfile());
  ASSERT_EQ(0, mgr->enable());
  ASSERT_TRUE(mgr->isEnabled());

  // test profile when enabled
  ASSERT_EQ(0, mgr->beginProfile("test2"));
  ASSERT_FALSE(!mgr->getCurrentProfile());
  ASSERT_TRUE(mgr->getCurrentProfile()->isRoot());
  ASSERT_EQ("test2", mgr->getCurrentProfile()->name);
  ASSERT_NE(0, mgr->disable()); // cannot change
  ASSERT_NE(0, mgr->enable());
}

TEST_F(ThreadLocalManagerTest, simple_profile) {
  ASSERT_EQ(0, mgr->enable());

  double start = currentTime();

  // check basic condition
  ASSERT_EQ(0, mgr->beginProfile("P1"));
  ASSERT_FALSE(!mgr->getCurrentProfile());
  ASSERT_TRUE(mgr->getCurrentProfile()->isRoot());
  EXPECT_EQ("P1", mgr->getCurrentProfile()->name);
  EXPECT_NE(0, mgr->getCurrentProfile()->start);
  EXPECT_EQ(0, mgr->getCurrentProfile()->end);

  // check message
  ASSERT_EQ(0, mgr->setMessage("M1"));
  EXPECT_EQ("M1", mgr->getCurrentProfile()->message);
  ASSERT_EQ(0, mgr->setMessage("M2"));
  EXPECT_EQ("M2", mgr->getCurrentProfile()->message);
  ASSERT_EQ(0, mgr->appendMessage("M3"));
  EXPECT_EQ("M2M3", mgr->getCurrentProfile()->message);

  // end profile
  ASSERT_TRUE(!mgr->getLastProfile());
  ASSERT_EQ(0, mgr->endProfile());
  ASSERT_TRUE(!mgr->getCurrentProfile());
  ASSERT_FALSE(!mgr->getLastProfile());
  ASSERT_TRUE(mgr->getLastProfile()->isRoot());
  ASSERT_TRUE(mgr->getLastProfile()->subprofiles.empty());
  ASSERT_NE(0, mgr->getLastProfile()->end);
  ASSERT_EQ("P1", mgr->getLastProfile()->name);
  ASSERT_EQ("M2M3", mgr->getLastProfile()->message);

  // check time
  double end = currentTime();
  EXPECT_LE(start, mgr->getLastProfile()->start);
  EXPECT_LE(mgr->getLastProfile()->end, end);
  EXPECT_LE(mgr->getLastProfile()->start, mgr->getLastProfile()->end);
}

TEST_F(ThreadLocalManagerTest, nested_profile) {
  ASSERT_EQ(0, mgr->enable());

  // begin profiles
  ASSERT_EQ(0, mgr->beginProfile("P1"));
  ASSERT_FALSE(!mgr->getCurrentProfile());
  ASSERT_TRUE(mgr->getCurrentProfile()->isRoot());
  ASSERT_EQ(0, mgr->setMessage("M1"));

  ASSERT_EQ(0, mgr->beginProfile("P2"));
  ASSERT_FALSE(!mgr->getCurrentProfile());
  ASSERT_FALSE(mgr->getCurrentProfile()->isRoot());
  ASSERT_TRUE(!mgr->getLastProfile());
  ASSERT_EQ(0, mgr->setMessage("M2"));

  ASSERT_EQ(0, mgr->beginProfile("P3"));
  ASSERT_FALSE(!mgr->getCurrentProfile());
  ASSERT_FALSE(mgr->getCurrentProfile()->isRoot());
  ASSERT_TRUE(!mgr->getLastProfile());
  ASSERT_EQ(0, mgr->setMessage("M3"));

  // end profiles
  ASSERT_EQ(0, mgr->endProfile()); // end P3
  ASSERT_TRUE(!mgr->getLastProfile());
  ASSERT_FALSE(!mgr->getCurrentProfile());
  ASSERT_EQ(0, mgr->endProfile()); // end P2
  ASSERT_TRUE(!mgr->getLastProfile());
  ASSERT_FALSE(!mgr->getCurrentProfile());
  ASSERT_EQ(0, mgr->endProfile()); // end P1
  ASSERT_FALSE(!mgr->getLastProfile()); // completely done
  ASSERT_TRUE(!mgr->getCurrentProfile()); // no current profile

  // check profiles
  ProfilePtr p1 = mgr->getLastProfile();
  EXPECT_EQ("P1", p1->name);
  EXPECT_EQ("M1", p1->message);
  ASSERT_EQ(1, p1->subprofiles.size());

  ProfilePtr p2 = p1->subprofiles[0];
  ASSERT_FALSE(!p2);
  EXPECT_TRUE(p1 == p2->parent);
  EXPECT_EQ("P2", p2->name);
  EXPECT_EQ("M2", p2->message);
  ASSERT_EQ(1, p2->subprofiles.size());

  ProfilePtr p3 = p2->subprofiles[0];
  ASSERT_FALSE(!p3);
  EXPECT_TRUE(p2 == p3->parent);
  EXPECT_EQ("P3", p3->name);
  EXPECT_EQ("M3", p3->message);
  EXPECT_TRUE(p3->subprofiles.empty());

  // check times
  ASSERT_NE(0, p1->start);
  EXPECT_LE(p1->start, p2->start);
  EXPECT_LE(p2->start, p3->start);
  EXPECT_LE(p3->start, p3->end);
  EXPECT_LE(p3->end, p2->end);
  EXPECT_LE(p2->end, p1->end);
  EXPECT_NE(0, p1->end);
}

TEST_F(ThreadLocalManagerTest, multi_subprofile) {
  ASSERT_EQ(0, mgr->enable());

  // begin profiles
  ASSERT_EQ(0, mgr->beginProfile("P1"));
  ASSERT_FALSE(!mgr->getCurrentProfile());
  ASSERT_TRUE(mgr->getCurrentProfile()->isRoot());
  ASSERT_EQ(0, mgr->setMessage("M1"));

  // begin&end subprofiles P2
  ASSERT_EQ(0, mgr->beginProfile("P2"));
  ASSERT_FALSE(!mgr->getCurrentProfile());
  ASSERT_FALSE(mgr->getCurrentProfile()->isRoot());
  ASSERT_TRUE(!mgr->getLastProfile());
  ASSERT_EQ(0, mgr->setMessage("M2"));
  ASSERT_EQ(0, mgr->endProfile()); // end P2
  ASSERT_TRUE(!mgr->getLastProfile());
  ASSERT_FALSE(!mgr->getCurrentProfile());

  // begin&end subprofiles P2
  ASSERT_EQ(0, mgr->beginProfile("P3"));
  ASSERT_FALSE(!mgr->getCurrentProfile());
  ASSERT_FALSE(mgr->getCurrentProfile()->isRoot());
  ASSERT_TRUE(!mgr->getLastProfile());
  ASSERT_EQ(0, mgr->setMessage("M3"));
  ASSERT_EQ(0, mgr->endProfile()); // end P3
  ASSERT_TRUE(!mgr->getLastProfile());
  ASSERT_FALSE(!mgr->getCurrentProfile());

  // end P1
  ASSERT_EQ(0, mgr->endProfile()); // end P1
  ASSERT_FALSE(!mgr->getLastProfile()); // completely done
  ASSERT_TRUE(!mgr->getCurrentProfile()); // no current profile

  // check profiles
  ProfilePtr p1 = mgr->getLastProfile();
  EXPECT_EQ("P1", p1->name);
  EXPECT_EQ("M1", p1->message);
  ASSERT_EQ(2, p1->subprofiles.size());

  ProfilePtr p2 = p1->subprofiles[0];
  ASSERT_FALSE(!p2);
  EXPECT_TRUE(p1 == p2->parent);
  EXPECT_EQ("P2", p2->name);
  EXPECT_EQ("M2", p2->message);
  EXPECT_TRUE(p2->subprofiles.empty());

  ProfilePtr p3 = p1->subprofiles[1];
  ASSERT_FALSE(!p3);
  EXPECT_TRUE(p1 == p3->parent);
  EXPECT_EQ("P3", p3->name);
  EXPECT_EQ("M3", p3->message);
  EXPECT_TRUE(p3->subprofiles.empty());

  // check times
  ASSERT_NE(0, p1->start);
  EXPECT_LE(p1->start, p2->start);
  EXPECT_LE(p2->start, p2->end);
  EXPECT_LE(p2->end, p3->start);
  EXPECT_LE(p3->start, p3->end);
  EXPECT_LE(p3->end, p1->end);
  EXPECT_NE(0, p1->end);
}

} // namespace mtkw
