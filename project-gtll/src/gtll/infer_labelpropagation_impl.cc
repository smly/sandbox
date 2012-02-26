// infer_labelpropagation_impl.cc
#include "infer_labelpropagation_impl.hpp"

#include "sparsify.hpp"
#include "similar.hpp"

#include <iostream> // for debug
#include <iomanip>

namespace gll_tool {

Labels LabelPropagation::infer(const Mat& m, const Gr& gr, const Labels& y)
{
  // Similar::getWeight(m, 3, 0) --- weight from x_0 to x_3
  // Sparse::isLinked(gr, 3, 0) --- exist edge from x_0 to x_3
  assert(y.size() == m.size()); // todo: error handling
  assert(m.size() == gr.size()); // todo: error handling
  // init
  const size_t N = m.size();
  const size_t numLabel = getNumClass(y);
  assert(numLabel > 0); // todo: error handling

  // init unlabeled node index
  std::vector<unsigned int> sample_u, sample_l;
  for (unsigned int i = 0; i < N; i++) {
    if (y[i] > 0) sample_l.push_back(i);
    else sample_u.push_back(i);
  }

  // compute the diagonal degree matrix D^{-1}
  std::vector<double> diagInv(N, 0.0);
  getDiagInv(diagInv, m, gr);

  // for classification function (Y_u)
  typedef std::vector<std::vector<double> > cFuncMat;
  cFuncMat cFunc(numLabel+1, std::vector<double>(sample_u.size(), 0.0));

  // iteration
  // all we should do is Y_u <- T_{uu} Y_u + T_{ul} Y_l untill convergence
  double diff;
  unsigned int iter = 0;
  do {
    iter++;
    cFuncMat cFuncNext(numLabel+1, std::vector<double>(sample_u.size(), 0.0));
    // propagate
    for (unsigned int i = 0; i < sample_u.size(); i++) {
      if (diagInv[ sample_u[i] ] == 0.0) continue; // this is a singleton vertex!
      for (unsigned int k = 1; k <= numLabel; k++) { // 1-indexed!
        // calc T_{uu} Y_u for the class k, sample i
        double sum = 0.0;
        for (unsigned int j = 0; j < sample_u.size(); j++) {
          // W_{ii} = 0, which often works better
          if (i == j) continue;
          if (!Sparse::isLinked(gr, sample_u[i], sample_u[j])) continue;

          sum += (Similar::getWeight(m, sample_u[i], sample_u[j])
                  / diagInv[ sample_u[i] ]) * cFunc[k][j];
        }
        cFuncNext[k][i] += sum; // todo: validate underflow

        // calc T_{ul} Y_l for the class k, sample i
        sum = 0.0;
        for (unsigned int j = 0; j < sample_l.size(); j++) {
          if (!Sparse::isLinked(gr, sample_u[i], sample_l[j])) continue;
          if (static_cast<unsigned int>(y[ sample_l[j] ]) != k) continue;

          sum += (Similar::getWeight(m, sample_u[i], sample_l[j])
                  / diagInv[ sample_u[i] ]) * 1.0;
        }
        cFuncNext[k][i] += sum; // todo: validate underflow
      }
    }

    // calc diff
    diff = calcFnorm(cFunc, cFuncNext, numLabel);
    cFunc = cFuncNext;
  } while (diff > 1.0e-8); // todo: use user defined prec
  std::cout << diff << ", " << iter << std::endl;

  Labels yp = y;
  determine(yp, cFunc, sample_u, numLabel);

  return yp;
}

}
