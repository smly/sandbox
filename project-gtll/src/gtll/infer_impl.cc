// infer_impl.cc
#include "infer_impl.hpp"
#include "sparsify.hpp"
#include "similar.hpp"

#include <algorithm>
#include <vector>
#include <set>
#include <cmath>

namespace gll_tool {

Mat InferenceImpl::rowNormalize(const Mat& m) // todo: eliminate
{
  const size_t N = m.size();
  Mat normalized(N);

  return normalized;
}

// todo: error handling
// class label is indicated by positive number (1-indexed).
int InferenceImpl::getNumClass(const Labels& y)
{
  const size_t sz = y.size();
  if (sz == 0) return 0;

  int ret = *std::max_element(y.begin(), y.end());
  // labeled data not found
  if (ret < 1)
    return 0;
  // sorted y is NOT continuous numbers
  if (sz < static_cast<size_t>(ret))
    return 0;
  // 1-indexed
  if (find(y.begin(), y.end(), 0) != y.end())
    return 0;

  // checking sorted y is continuous numbers
  // (it is important for inference phase
  // to briefly construct minimum label matrix Y).
  std::set<int> lset;
  for (unsigned int i = 0; i < y.size(); i++)
    if (y[i] > 0) lset.insert( y[i] );
  if (lset.size() != static_cast<size_t>(ret))
    return 0;

  return ret;
}

void InferenceImpl::getNorGrLap(
    Mat& lap,
    const Mat& m,
    const Gr& gr)
{
  lap = m;
  const size_t N = m.size();
  assert(lap.size() == m.size()); // todo
  assert(m.size() == gr.size()); // todo
  assert(lap.size() > 0); // todo

  // D^{-\frac{1}{2}}
  std::vector<double> diagInvSqrt(N, 0.0);
  getDiagInv(diagInvSqrt, m, gr);
  for (unsigned int i = 0; i < N; i++) {
    if (diagInvSqrt[i] != 0.0) {
      diagInvSqrt[i] = 1.0 / sqrt(diagInvSqrt[i]);
    }
  }
  for (unsigned int i = 0; i < N - 1; i++) {
    for (unsigned int j = 0; i + j + 1 < N; j++) {
      unsigned int dst = i+j+1;
      if (i != dst && Sparse::isLinked(gr, i, dst)) {
        // W_{ij} = 0, so ij element of it is also zero.
        lap[i][j] *= (diagInvSqrt[i] * diagInvSqrt[dst]);
      }
    }
  }
}

void InferenceImpl::getDiagInv(
    std::vector<double>& diagInv,
    const Mat& m,
    const Gr& gr)
{
  assert(diagInv.size() == m.size());
  assert(m.size() == gr.size());

  const size_t N = diagInv.size();
  for (unsigned int i = 0; i < N; i++) {
    for (unsigned int j = 0; j < N; j++) {
      // W_{ii} = 0, which often works better
      if (i != j && Sparse::isLinked(gr, i, j)) {
        double w = Similar::getWeight(m, i, j);
        if ( w != 0.0 ) diagInv[i] += w; // todo: validate underflow
      }
    }
  }
}

void InferenceImpl::determine(
    Labels& yp,
    const std::vector<std::vector<double> >& cFunc,
    const std::vector<unsigned int>& sample_u,
    const size_t numberLabel)
{
  // todo: error handling & validation
  for (unsigned int i = 0; i < sample_u.size(); i++) {
    unsigned int k_ = 0;
    double k_max = -1;
    for (unsigned int k = 1; k <= numberLabel; k++) {
      if (k_max < cFunc[k][i]) {
        k_max = cFunc[k][i];
        k_ = k;
      }
    }
    yp[ sample_u[i] ] = k_;
  }
}

double InferenceImpl::calcFnorm(
    const std::vector<std::vector<double> >& cFunc,
    const std::vector<std::vector<double> >& cFuncNext,
    const size_t numClass)
{
  assert(0 < numClass);
  assert(cFunc.size() == numClass + 1);
  assert(cFunc.size() == cFuncNext.size());
  assert(cFunc[0].size() == cFuncNext[0].size());

  const size_t N = cFunc[0].size();
  double diff = 0.0;
  for (unsigned int i = 1; i <= numClass; i++) {
    for (unsigned int j = 0; j < N; j++) {
      diff += cFuncNext[i][j] > cFunc[i][j] ?
          cFuncNext[i][j] - cFunc[i][j] :
          cFunc[i][j] - cFuncNext[i][j];
    }
  }

  return diff;
}

}
