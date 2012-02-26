#include <gtest/gtest.h>
#include "gll.hpp"
#include "testing/utils.hpp"

#include <iomanip>
#include <iostream>
#include <cmath>

namespace gll_tool {

class WithGaussianKernelTest : public ::testing::Test {
 protected:
  WithGaussianKernelTest() { }
  virtual void SetUp() {
    testfile = Path::TestSourceDir() + "/testdata/samples/sample_two.w";
    accessible = File::Exists(testfile);
  }

  std::string testfile;
  bool accessible;
};

TEST_F(WithGaussianKernelTest, InitTest) {
  ASSERT_TRUE(accessible);
  Config conf;
  gll model;
  ASSERT_TRUE(0 == model.load(testfile));

  model.load(testfile);
  std::vector<int> ulabel(model.getUnlabeledPos());

  Mat m;
  Gr gr;
  Labels y;
  m  = model.buildMatrix(conf, GKERN_L2NORM);
  EXPECT_DOUBLE_EQ(exp(-1 * 4  / 2.0), Similar::getWeight(m, 0, 1));
  EXPECT_DOUBLE_EQ(exp(-1 * 10 / 2.0), Similar::getWeight(m, 0, 2));
  EXPECT_DOUBLE_EQ(exp(-1 * 25 / 2.0), Similar::getWeight(m, 0, 3));

  conf.gaussianKernelDelta = 0.1;
  m  = model.buildMatrix(conf, GKERN_L2NORM);
  // more accurate
  //  EXPECT_DOUBLE_EQ(exp(-1 * 10 / 0.02), Similar::getWeight(m, 0, 2));
  // todo: fix it
  //  EXPECT_DOUBLE_EQ(exp(-1 * 10 / (2.0 * pow(conf.gaussianKernelDelta, 2)))
  //                   , Similar::getWeight(m, 0, 2));

  // more accurate
  //  EXPECT_DOUBLE_EQ(exp(-1 * 25 / 0.02), Similar::getWeight(m, 0, 3));
  // todo: fix it
  EXPECT_DOUBLE_EQ(exp(-1 * 25 / (2.0 * pow(conf.gaussianKernelDelta, 2)))
                   , Similar::getWeight(m, 0, 3));

  gr = model.buildGraph(KNN, 2, m);
  EXPECT_EQ(true, Sparse::isLinked(gr, 0, 1));
  EXPECT_EQ(true, Sparse::isLinked(gr, 0, 2));
  EXPECT_EQ(false, Sparse::isLinked(gr, 0, 3));
  EXPECT_EQ(false, Sparse::isLinked(gr, 0, 4));

  /*
  std::cout.setf(std::ios::fixed, std::ios::floatfield);
  for (unsigned int i = 0; i < m.size(); i++) {
    for (unsigned int j = 0; j < m.size(); j++) {
      std::cout << std::setw(3) << Similar::getWeight(m, i, j) << ' ';
    }
    std::cout << std::endl;
  }
  std::cout.unsetf(std::ios::fixed);
  */
}

/*
TEST(GetNormalizedGraphLaplacianTest, LocalGlobalTest) {
  Config conf;
  Similar measure(conf, GKERN_L2NORM);
  fv a, b;
  a.push_back(std::make_pair(1, 0.2));
  a.push_back(std::make_pair(2, 0.2));
  b.push_back(std::make_pair(2, 0.1));
  EXPECT_DOUBLE_EQ(0.97530991202833262, measure.sim(a, b));
}
*/

};
