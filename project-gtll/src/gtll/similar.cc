#include "similar.hpp"

#include <iostream>
#include <cstdlib>
#include <cmath>

namespace gll_tool {

Similar::Similar(
    const Config conf,
    const SimilarImplType impl_type)
    : type_(impl_type), conf_(conf)
{
  switchMeasure(type_, conf_);
}

double Similar::getWeight(\
    const Mat& m,
    const unsigned int i,
    const unsigned int j)
{
  const unsigned int n = m.size();
  if (i >= n || j >= n || i == j) return 0.0;
  if (i > j) return Similar::getWeight(m, j, i);
  if (m[i].size() <= j-i-1) return 0.0;

  return m[i][j-i-1];
}

double Similar::sim(\
    const fv& rhs,
    const fv& lhs)
{
  return impl_->sim(rhs, lhs);
}

Mat Similar::calcMatrix(const sample_v& s)
{
  const unsigned int n = s.size(); // number of samples
  if (n == 0) return Mat(0);

  Mat m(n);
  for (unsigned int i = 0; i < n - 1; i++) {
    m[i].resize(n - i - 1);
    for (unsigned int j = 0; i + j + 1 < n; j++) {
      // A_{ij} = m[i][i-j-1] s.t. i < j
      m[i][j] = sim( s[i].second, s[i+j+1].second );
    }
    // calc
  }

  return m;
}

void Similar::switchMeasure(
    const SimilarImplType impl_type,
    const Config conf)
{
  if (impl_type == IPROD) {
    boost::scoped_ptr< SimilarImpl > tmp(new InnerProductImpl(conf));
    impl_.swap(tmp);
  } else if (type_ == BINARY) {
    boost::scoped_ptr< SimilarImpl > tmp(new BinaryImpl(conf));
    impl_.swap(tmp);
  } else if (type_ == GKERN_L2NORM) {
    boost::scoped_ptr< SimilarImpl > tmp(new GaussianKernel<L2Norm>(conf));
    impl_.swap(tmp);
  } else if (type_ == GKERN_CHISQ) {
    boost::scoped_ptr< SimilarImpl > tmp(new GaussianKernel<ChiSquare>(conf));
    impl_.swap(tmp);
  }// todo: add else
}

double Similar::BinaryImpl::sim(
    const fv& rhs,
    const fv& lhs)
{
  double s = 0.0;
  fv::const_iterator rit = rhs.begin();
  fv::const_iterator lit = lhs.begin();
  while (rit != rhs.end() && lit != lhs.end()) {
    if (rit->first == lit->first) {
      s += 1.0;
      rit++;
      lit++;
    } else if (rit->first > lit->first) {
      lit++;
    } else {
      rit++;
    }
  }

  return s;
}

double Similar::InnerProductImpl::sim(\
    const fv& rhs,
    const fv& lhs)
{
  double s = 0.0;
  fv::const_iterator rit = rhs.begin();
  fv::const_iterator lit = lhs.begin();
  while (rit != rhs.end() && lit != lhs.end()) {
    if (rit->first == lit->first) {
      s += rit->second * lit->second;
      rit++;
      lit++;
    } else if (rit->first > lit->first) {
      lit++;
    } else {
      rit++;
    }
  }

  return s;
}

double Similar::CosineImpl::sim(\
    const fv& rhs,
    const fv& lhs)
{
  double s = 0.0, rs = 0.0, ls = 0.0;
  fv::const_iterator rit = rhs.begin();
  fv::const_iterator lit = lhs.begin();
  while (rit != rhs.end() && lit != lhs.end()) {
    if (rit->first == lit->first) {
      s += rit->second * lit->second;
      rit++;
      lit++;
    } else if (rit->first > lit->first) {
      ls += lit->second;
      lit++;
    } else {
      rs += rit->second;
      rit++;
    }
  }
  while (rit != rhs.end()) {
    rs += rit->second;
    rit++;
  }
  while (lit != lhs.end()) {
    ls += lit->second;
    lit++;
  }

  return s / (rs * ls);
}

template <typename T>
double Similar::GaussianKernel<T>::evaluate(double dist)
{
  double delta = conf_.gaussianKernelDelta;
  return exp(-1 * dist / (2 * delta * delta));
}

template <>
double Similar::GaussianKernel<L2Norm>::distance(
    const fv& rhs,
    const fv& lhs)
{
  double dist = 0.0;
  fv::const_iterator rit = rhs.begin();
  fv::const_iterator lit = lhs.begin();
  while (rit != rhs.end() && lit != lhs.end()) {
    if (rit->first == lit->first) {
      double elem = rit->second - lit->second;
      dist += elem * elem;
      rit++;
      lit++;
    } else if (rit->first > lit->first) {
      dist += (lit->second) * (lit->second);
      lit++;
    } else {
      dist += (rit->second) * (rit->second);
      rit++;
    }
  }
  while (rit != rhs.end()) {
    dist += rit->second * rit->second;
    rit++;
  }
  while (lit != lhs.end()) {
    dist += lit->second * lit->second;
    lit++;
  }

  return dist;
}

template <>
double Similar::GaussianKernel<ChiSquare>::distance(
    const fv& rhs,
    const fv& lhs)
{
  double dist = 0.0;
  fv::const_iterator rit = rhs.begin();
  fv::const_iterator lit = lhs.begin();
  while (rit != rhs.end() && lit != lhs.end()) {
    if (rit->first == lit->first) {
      double elem = rit->second - lit->second;
      double denom = rit->second + lit->second;
      dist += (denom == 0.0) ? 0.0 : (elem * elem) / denom;

      rit++;
      lit++;
    } else if (rit->first > lit->first) {
      dist += lit->second;
      lit++;
    } else {
      dist += rit->second;
      rit++;
    }
  }
  while (rit != rhs.end()) {
    dist += rit->second;
    rit++;
  }
  while (lit != lhs.end()) {
    dist += lit->second;
    lit++;
  }

  return dist;
}

template <typename T>
double Similar::GaussianKernel<T>::sim(
    const fv& rhs,
    const fv& lhs)
{
  double dist = distance(rhs, lhs);
  return evaluate(dist);;
}

} // end
