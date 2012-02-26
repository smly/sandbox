#include <gtest/gtest.h>
#include "testing/utils.hpp"
#include "gll.hpp"

namespace gll_tool {

class SimilarLoadTest : public ::testing::Test {
 protected:
  SimilarLoadTest() { }
  virtual void SetUp() {
    testfile = Path::TestSourceDir() + "/testdata/samples/sample.w";
    accessible = File::Exists(testfile);
  }

  std::string testfile;
  bool accessible;
};

TEST(SimilarityTest, SampleFileTest) {
  Config conf;
  fv a1, a2, a3, a4, a5, a6;
  a1.push_back(std::make_pair(1, 0.1));
  a1.push_back(std::make_pair(4, 0.1));
  a1.push_back(std::make_pair(6, 0.12));

  a2.push_back(std::make_pair(1, 0.1));
  a2.push_back(std::make_pair(5, 0.4));
  a2.push_back(std::make_pair(6, 0.2));

  a3.push_back(std::make_pair(1, 0.1));
  a3.push_back(std::make_pair(4, 0.12));
  a3.push_back(std::make_pair(6, 0.1));

  a4.push_back(std::make_pair(2, 0.1));
  a4.push_back(std::make_pair(5, 0.1));
  a4.push_back(std::make_pair(6, 0.1));

  a5.push_back(std::make_pair(2, 0.1));
  a5.push_back(std::make_pair(4, 0.1));
  a5.push_back(std::make_pair(5, 0.1));

  a6.push_back(std::make_pair(2, 0.1));
  a6.push_back(std::make_pair(3, 0.1));
  a6.push_back(std::make_pair(4, 0.1));

  sample_v sm;
  sm.push_back(std::make_pair(1,  a1));
  sm.push_back(std::make_pair(1,  a2));
  sm.push_back(std::make_pair(-1, a3));
  sm.push_back(std::make_pair(2,  a4));
  sm.push_back(std::make_pair(-1, a5));
  sm.push_back(std::make_pair(2,  a6));
  Similar measure(conf, IPROD);
  EXPECT_DOUBLE_EQ(0.034, measure.sim(a1,a2));
  EXPECT_DOUBLE_EQ(0.030, measure.sim(a2,a3));
  EXPECT_DOUBLE_EQ(0.060, measure.sim(a2,a4));
  EXPECT_DOUBLE_EQ(0.040, measure.sim(a2,a5));
  EXPECT_DOUBLE_EQ(0.000, measure.sim(a2,a6)); // missing link
}

TEST(InnerProductTest, CalcSimilarTest) {
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
  EXPECT_EQ(2, m.size())
      << "size of matrix";
  EXPECT_DOUBLE_EQ(0.02, m[0][0])
      << "similarity between node 0(a) and node 1(b)";
}

TEST(BinarySimilarTest, CalcSimilarTest) {
  Config conf;
  Similar measure(conf, BINARY);
  fv a, b;
  a.push_back(std::make_pair(1, 0.2));
  a.push_back(std::make_pair(4, 0.2));
  b.push_back(std::make_pair(4, 0.1));
  EXPECT_DOUBLE_EQ(1.0, measure.sim(a, b));
  sample_v sm;
  sm.push_back(std::make_pair(1, a));
  sm.push_back(std::make_pair(2, b));
  Mat m = measure.calcMatrix(sm);
  EXPECT_EQ(2, m.size())
      << "size of matrix";
  EXPECT_DOUBLE_EQ(1.0, m[0][0])
      << "similarity between node 0(a) and node 1(b)";
}

TEST(GaussianL2NormSimilarTest, CalcSimilarTest) {
  Config conf;
  conf.gaussianKernelDelta = 1.0;
  Similar measure(conf, GKERN_L2NORM);
  fv a, b;
  a.push_back(std::make_pair(1, 0.2));
  a.push_back(std::make_pair(2, 0.2));
  b.push_back(std::make_pair(2, 0.1));
  EXPECT_DOUBLE_EQ(0.97530991202833262, measure.sim(a, b));
}

}
