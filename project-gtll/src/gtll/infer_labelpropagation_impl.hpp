#ifndef INFER_LABELPROPAGATION_IMPL_HPP
#define INFER_LABELPROPAGATION_IMPL_HPP
// infer_labelpropagation_impl.hpp

#include "gtll/infer_impl.hpp"
#include "gtll/data.hpp"

namespace gll_tool {

class LabelPropagation : public InferenceImpl {
 public:
  Labels infer(const Mat& m, const Gr& gr, const Labels& y);
};

}

#endif
