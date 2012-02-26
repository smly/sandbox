#ifndef SIMILAR_HPP
#define SIMILAR_HPP

#include <boost/scoped_ptr.hpp>
#include "gtll/data.hpp"

#include "gtllconfig.h"

#ifdef HAVE_GTEST_GTEST_PROD_H
#ifndef __GLL_PRIVATE_TEST
#define __GLL_PRIVATE_TEST
#endif
#include <gtest/gtest_prod.h>
#endif

namespace gll_tool {

typedef enum SimilarImplType {
  IPROD = 0, // inner product
  COSINE,    // cosine similarity
  GKERN_L2NORM, // gaussian kernel (L2 norm)
  GKERN_CHISQ, // gaussian kernel (Chi-square)
  BINARY,    // binary
} SimilarImplType;

// distance used for gaussian kernel
struct L2Norm {};
struct ChiSquare {};

class Similar {
  // implementation for calculating various similarity
  class SimilarImpl;
  class InnerProductImpl;
  class BinaryImpl;
  class CosineImpl;
  template <typename T> class GaussianKernel;

  boost::scoped_ptr< SimilarImpl > impl_;
  SimilarImplType type_;
  Config conf_;

 public:
  Similar(
      const Config conf,
      const SimilarImplType impl_type);

  // alias to impl_->sim
  double sim(const fv& rhs, const fv& lhs);
  // switch similarity measure
  void switchMeasure(
      const SimilarImplType impl_type,
      const Config conf);
  // return weighted matrix
  Mat calcMatrix(const sample_v& s);
  // get similarity weight between node i and node j
  static double getWeight(const Mat& m,
                          const unsigned int i,
                          const unsigned int j);
};

class Similar::SimilarImpl {
 public:
  // similarity measure
  virtual double sim(const fv& rhs, const fv& lhs) = 0;
};

class Similar::InnerProductImpl : public Similar::SimilarImpl {
 public:
  const Config conf_;
  InnerProductImpl() { Config conf_; }
  InnerProductImpl(const Config conf) : conf_(conf) { }

  double sim(const fv& rhs, const fv& lhs);
};

class Similar::BinaryImpl : public Similar::SimilarImpl {
 public:
  const Config conf_;
  BinaryImpl() { Config conf_; }
  BinaryImpl(const Config conf) : conf_(conf) { }

  double sim(const fv& rhs, const fv& lhs);
};

class Similar::CosineImpl : public Similar::SimilarImpl {
 public:
  const Config conf_;
  CosineImpl() { Config conf_; }
  CosineImpl(const Config conf) : conf_(conf) { }

  double sim(const fv& rhs, const fv& lhs);
};

template <typename T>
class Similar::GaussianKernel : public Similar::SimilarImpl {
 private:
  // compute \exp{-\frac{d(x,y)}{2 \delta^2}}
  double evaluate(const double dist);
  double distance(const fv& rhs, const fv& lhs);
 public:
  const Config conf_;
  GaussianKernel() { Config conf_; }
  GaussianKernel(const Config conf) : conf_(conf) { }
  double sim(const fv& rhs, const fv& lhs);
};

} // end namespace

#endif
