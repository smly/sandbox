#include <gtest/gtest.h>

namespace {

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
