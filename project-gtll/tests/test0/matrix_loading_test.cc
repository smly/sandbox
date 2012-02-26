#include <gtest/gtest.h>
#include <fstream>
#include "gll.hpp"
#include "testutils.h"

namespace gll_tool {

class LoadTest : public ::testing::Test {
 protected:
  LoadTest() { }
};

TEST_F(LoadTest, LoadFeatureVector) {
  using gll_tool::return_one;
  using gll_tool::gll;
  using gll_tool::Config;

  EXPECT_EQ(1, return_one(10));

  // set parameter
  Config conf;
  conf.k = 10;
  conf.induction = gll_tool::LP;
  EXPECT_EQ(conf.k, 10);
  const char* testfile = "/tmp/gll_gtest_loadtest.dat";
  // remove test file
}

class FooTest : public ::testing::Test {
 protected:
  FooTest() {
    
  }

  virtual ~FooTest() {
    
  }

  virtual void SetUp() {

  }

  virtual void TearDown() {

  }
  
};

TEST_F(FooTest, MethodBarDoesAbc) {
  EXPECT_EQ(0, 0);
}

TEST_F(FooTest, DoesXyz) {
  EXPECT_EQ(0, 0);
}

}
