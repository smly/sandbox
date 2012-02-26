#include "infer.hpp"

#include "similar.hpp"
#include "sparsify.hpp"
#include <iostream> // for debug
#include <algorithm>
#include <set>

namespace gll_tool {

Infer::Infer(const InferenceAlgorithm type)
    : type_(type) {
  switchMethod(type_);
}

void Infer::switchMethod(const InferenceAlgorithm impl_type)
{
  if (impl_type == LP) {
    boost::scoped_ptr< InferenceImpl > tmp(new LabelPropagation());
    impl_.swap(tmp);
  } else if (impl_type == LGC) {
    boost::scoped_ptr< InferenceImpl > tmp(new LocalGlobal());
    impl_.swap(tmp);
  }// todo: add else
}

Labels Infer::infer(const Mat& m, const Gr& gr, const Labels& y)
{
  return impl_->infer(m, gr, y);
}

}// namespace
