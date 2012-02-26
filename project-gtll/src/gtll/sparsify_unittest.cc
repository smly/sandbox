#include <gtest/gtest.h>
#include "testing/utils.hpp"
#include "gll.hpp"

namespace gll_tool {

class SparsifyLoadTest : public ::testing::Test {
 protected:
  SparsifyLoadTest() { }
  virtual void SetUp() {
    testfile = Path::TestSourceDir() + "/testdata/samples/sample.w";
    accessible = File::Exists(testfile);
  }

  std::string testfile;
  bool accessible;
};

TEST_F(SparsifyLoadTest, SparsifyLoadTest) {
  ASSERT_TRUE(accessible)
      << "testfile accessible";

  Config conf;
  gll model;
  Mat m;
  Gr gr;
  EXPECT_EQ(0, model.load(testfile))
      << "model.load done";
  m = model.buildMatrix(conf, IPROD);
  gr = model.buildGraph(KNN, 3, m);
  EXPECT_EQ(4, gr[0].count()); // number of neighbor node s.t. i < j
  EXPECT_EQ(3, gr[1].count()); // number of neighbor node s.t. i < j
  EXPECT_EQ(2, gr[2].count()); // number of neighbor node s.t. i < j
  EXPECT_EQ(1, gr[3].count()); // number of neighbor node s.t. i < j
  EXPECT_EQ(1, gr[3][0]); // 3, 4
  EXPECT_EQ(0, gr[3][1]); // 3, 5
  EXPECT_EQ(true,  Sparse::isLinked(gr, 3, 4));
  EXPECT_EQ(false, Sparse::isLinked(gr, 3, 5));
  EXPECT_DOUBLE_EQ(0.012, Similar::getWeight(m, 3, 0));
  EXPECT_DOUBLE_EQ(0.060, Similar::getWeight(m, 3, 1));
  EXPECT_DOUBLE_EQ(0.010, Similar::getWeight(m, 3, 2));
  EXPECT_DOUBLE_EQ(0.020, Similar::getWeight(m, 3, 4));
  EXPECT_DOUBLE_EQ(0.010, Similar::getWeight(m, 3, 5));
  EXPECT_EQ(1, gr[4].count()); // number of neighbor node s.t. i < j
  EXPECT_EQ(0, gr[5].count()); // number of neighbor node s.t. i < j
}

TEST(DynamicBitsetTest, SparsifyTest) {
  Config conf;
  Similar measure(conf, IPROD);
  fv a, b;
  a.push_back(std::make_pair(1, 0.2));
  a.push_back(std::make_pair(4, 0.2));
  b.push_back(std::make_pair(4, 0.1));
  EXPECT_DOUBLE_EQ(0.02, measure.sim(a, b));
  sample_v sm;
  sm.push_back(std::make_pair(1, a));
  sm.push_back(std::make_pair(2, b));
  Mat m = measure.calcMatrix(sm);
  Sparse sparsify(KNN, 3);
  Gr g = sparsify.calcSparsify(m, 3);
  EXPECT_EQ(2, g.size())
      << "sample size is 2";
  EXPECT_EQ(1, g[0].count())
      << "degree of 0 is 1";
  EXPECT_EQ(0, g[1].count())
      << "degree of 1 is 0";
  EXPECT_EQ(1, g[0][0])
      << "connectivity between 0 and 1";
}

}
