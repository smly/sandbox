#ifndef GLL_HPP
#define GLL_HPP

// libgrll -- graph labeling library
// Kohei Ozaki, 2010

// references
// GFHF
// [1] Zhu, X., Ghahramani, Z., & Lafferty, J. (2003). Semi-supervised learning using gaussian fields and harmonic functions. Int. Conf. on Mach. Learn. (pp.912--919)

// LGC
// [2] Zhou, D., Bousquet, O., Lal, T., Weston, J., & Scholkopf, B. (2004). Learning with local and global consistency. The Neural Information Processing Systems (pp. 321--328).

// GTAM
// [3]

#include "gtll/data.hpp"
#include "gtll/similar.hpp"
#include "gtll/sparsify.hpp"
#include "gtll/infer.hpp"

#include "gtllconfig.h"

#ifdef HAVE_GTEST_GTEST_PROD_H
#ifndef __GLL_PRIVATE_TEST
#define __GLL_PRIVATE_TEST
#endif
#include <gtest/gtest_prod.h>
#endif

namespace gll_tool {

#ifdef __GLL_PRIVATE_TEST
class LoadTest;
#endif

class gll {
 private:
#ifdef __GLL_PRIVATE_TEST
  FRIEND_TEST(LoadTest, AppendTest);
  FRIEND_TEST(LoadTest, LoadFeatureVector);
#endif

  sample_v mat_;
  size_t dim_;
  size_t N_;
  int labmax_;

  void appendLine(fv &v, int &lab, const std::string &line);

 public:
  gll(): dim_(0), N_(0), labmax_(0) { };
  ~gll() { };

  size_t getNum(void) { return N_; };
  size_t getDimension(void) { return dim_; };
  int getLabelMax(void) { return labmax_; };
  int load(const std::string& filename);
  int loadLines(const std::vector<std::string>& lines);
  std::vector<int> getUnlabeledPos(void) const;

  Mat buildMatrix(
      const Config& conf,
      const SimilarImplType type) const;
  Gr buildGraph(
      const SparsifyImplType type,
      const size_t n,
      const Mat& m);
  Labels infer(
      const InferenceAlgorithm type,
      const Mat& m, // weight matrix
      const Gr& gr); // sparsed adjacency matrix
};

struct fv_elem_comp {
  bool operator()(const fv_elem& lhs, const fv_elem& rhs) const
  {
    return lhs.first < rhs.first;
  }
};

}

#endif
