#include <iostream>
#include <gtest/gtest.h>
#include "gll.hpp"
#include "testing/utils.hpp"

namespace gll_tool {

class LoadTest : public ::testing::Test {
 protected:
  LoadTest() { }
  virtual void SetUp() {
    testfile = Path::TestSourceDir() + "/testdata/samples/sample.w";
    accessible = File::Exists(testfile);
  }

  std::string testfile;
  bool accessible;
};

TEST_F(LoadTest, AccessTestFile) {
  EXPECT_EQ(true, accessible)
      << "accessing testfile";
}

TEST_F(LoadTest, LoadFeatureVector) {
  ASSERT_TRUE(accessible)
      << "AccessTestFile failed";

  Config conf;
  gll model;
  EXPECT_EQ(0, model.load(testfile))
      << "loading testfile";
  EXPECT_EQ(6, model.getNum())
      << "number of samples in testfile";
  EXPECT_EQ(6, model.getDimension())
      << "number of dimensions in testfile";
  EXPECT_EQ(2, model.getLabelMax())
      << "maximum label number in testfile";

  EXPECT_EQ(6,  model.mat_.size());
  EXPECT_EQ(1,  model.mat_[0].first);
  EXPECT_EQ(1,  model.mat_[1].first);
  EXPECT_EQ(-1, model.mat_[2].first);
  EXPECT_EQ(2,  model.mat_[3].first);
  EXPECT_EQ(-1, model.mat_[4].first);
  EXPECT_EQ(2,  model.mat_[5].first);

  EXPECT_EQ(3, model.mat_[0].second.size());
  EXPECT_EQ(3, model.mat_[1].second.size());
  EXPECT_EQ(3, model.mat_[2].second.size());
  EXPECT_EQ(3, model.mat_[3].second.size());
  EXPECT_EQ(3, model.mat_[4].second.size());
  EXPECT_EQ(3, model.mat_[5].second.size());
  Mat m = model.buildMatrix(conf, IPROD);
  EXPECT_EQ(5, m[0].size());
  EXPECT_EQ(4, m[1].size());
  EXPECT_EQ(1, m[4].size());
  EXPECT_EQ(0, m[5].size());

  Similar simi(conf, IPROD);
  // check sim(i,j) == m[i][j-i-1] s.t. i < j
  EXPECT_DOUBLE_EQ(0.034, m[0][0]);
  EXPECT_DOUBLE_EQ(simi.sim(model.mat_[0].second, model.mat_[1].second),
                   m[0][0]);
  EXPECT_DOUBLE_EQ(simi.sim(model.mat_[0].second, model.mat_[2].second),
                   m[0][1]);
  EXPECT_DOUBLE_EQ(simi.sim(model.mat_[0].second, model.mat_[3].second),
                   m[0][2]);
  EXPECT_DOUBLE_EQ(simi.sim(model.mat_[0].second, model.mat_[5].second),
                   m[0][4]);

  // sim(x_2, x_3) == 0.030
  EXPECT_DOUBLE_EQ(simi.sim(model.mat_[1].second, model.mat_[2].second),
                   0.030);
  EXPECT_DOUBLE_EQ(0.030, m[1][0]);

  // load check (weight)
  EXPECT_DOUBLE_EQ(0.1, model.mat_[1].second[0].second);
  EXPECT_DOUBLE_EQ(0.4, model.mat_[1].second[1].second);
  EXPECT_DOUBLE_EQ(0.2, model.mat_[1].second[2].second);
  EXPECT_DOUBLE_EQ(0.1, model.mat_[2].second[0].second);
  EXPECT_DOUBLE_EQ(0.12, model.mat_[2].second[1].second);
  EXPECT_DOUBLE_EQ(0.1, model.mat_[2].second[2].second);
  // load check (feature id)
  EXPECT_EQ(1, model.mat_[1].second[0].first);
  EXPECT_EQ(5, model.mat_[1].second[1].first);
  EXPECT_EQ(6, model.mat_[1].second[2].first);
  EXPECT_EQ(1, model.mat_[2].second[0].first);
  EXPECT_EQ(4, model.mat_[2].second[1].first);
  EXPECT_EQ(6, model.mat_[2].second[2].first);

  Gr gr = model.buildGraph(FULL, 3, m);
  EXPECT_EQ(5, gr[0].count());
  EXPECT_EQ(3, gr[1].count()); // number of neighbor node s.t. i < j
  EXPECT_EQ(3, gr[2].count()); // number of neighbor node s.t. i < j
  EXPECT_EQ(2, gr[3].count()); // number of neighbor node s.t. i < j
  EXPECT_EQ(1, gr[4].count()); // number of neighbor node s.t. i < j
  EXPECT_EQ(0, gr[5].count()); // number of neighbor node s.t. i < j
}

}
