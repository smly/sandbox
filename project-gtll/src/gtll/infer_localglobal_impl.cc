// infer_localglobal_impl.cc
#include "infer_localglobal_impl.hpp"

#include "sparsify.hpp"
#include "similar.hpp"

#include <iostream> // for debug
#include <iomanip>
#include <cmath>

namespace gll_tool {

Labels LocalGlobal::infer(const Mat& m, const Gr& gr, const Labels& y)
{
  assert(y.size() == m.size());// todo: error handling
  assert(m.size() == gr.size());
  // init
  const size_t N = m.size();
  const size_t numClass = getNumClass(y);
  assert(numClass > 0);

  // init unlabeled node index
  std::vector<unsigned int> sample_u, sample_l;
  for (unsigned int i = 0; i < N; i++) {
    if (y[i] > 0) sample_l.push_back(i);
    else sample_u.push_back(i);
  }

  // compute the normalized graph laplacian
  std::vector<double> diagInvSqrt(N, 0.0);
  getDiagInv(diagInvSqrt, m, gr);

  Mat lap;
  getNorGrLap(lap, m, gr);

  // for classification function (Y)
  typedef std::vector<std::vector<double> > cFuncMat;
  cFuncMat cFunc(numClass+1, std::vector<double>(N, 0.0));

  // iteration
  // Y <- \alpha L Y + (1- \alpha) Y_{init}
  const double alpha = 0.99; // todo: use user defined parameter
  double diff;
  unsigned int iter = 0;
  do {
    iter++;
    cFuncMat cFuncNext(numClass+1, std::vector<double>(N, 0.0));
    // propagate
    for (unsigned int i = 0; i < N; i++) {
      for (unsigned int k = 1; k <= numClass; k++) {
        // calc Y += \alpha L Y
        double sum = 0.0;
        for (unsigned int j = 0; j < N; j++) {
          if (i == j) continue;
          if (!Sparse::isLinked(gr, i, j)) continue;

          sum += Similar::getWeight(lap, i, j) * cFunc[k][j];
        }
        cFuncNext[k][i] += sum * alpha;
      }
    }
    // compute the local affector (Y += (1-\alpha) Y_{init})
    for (unsigned int j = 0; j < sample_l.size(); j++) {
      const unsigned int k = y[ sample_l[j] ];
      if (k > 0) cFuncNext[k][ sample_l[j] ] += (1.0 - alpha);
    }

    diff = calcFnorm(cFunc, cFuncNext, numClass);
    cFunc = cFuncNext;

  } while (diff > 1.0e-8); // todo: use user defined prec

  std::cout << diff << ", " << iter << std::endl; // todo: use better way to supply the info

  Labels yp = y;
  determine(yp, cFunc, sample_u, numClass);

  return yp;
}

};
