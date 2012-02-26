#ifndef INFER_IMPL_HPP
#define INFER_IMPL_HPP
// infer_impl.hpp

#include "gtll/data.hpp"
#include <vector>

namespace gll_tool {

class InferenceImpl {

 public:
  virtual Labels infer(
      const Mat& m,
      const Gr& gr,
      const Labels& y) = 0;

  // return row-normalized matrix
  Mat rowNormalize(const Mat& m);
  // get number of class
  int getNumClass(const Labels& y);
  // normalized graph laplacian L <- D^{-\frac{1}{2}} W D^{-\frac{1}{2}}
  void getNorGrLap(
      std::vector<std::vector<double> >& lap,
      const Mat& m,
      const Gr& gr);
  // compute the diagonal degree matrix D^{-1}
  void getDiagInv(
      std::vector<double>& diag,
      const Mat& m,
      const Gr& gr);
  // determine class label for unlabeled data
  void determine(
      Labels& y,
      const std::vector<std::vector<double> >& cFunc,
      const std::vector<unsigned int>& sample_u,
      const size_t numberLabel);
  // calc diff of cFunc and cFunc'
  double calcFnorm(
      const std::vector<std::vector<double> >& cFunc,
      const std::vector<std::vector<double> >& cFuncNext,
      const size_t numClass);

};

}

#endif
