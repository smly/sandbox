#ifndef INFER_LOCALGLOBAL_IMPL_HPP
#define INFER_LOCALGLOBAL_IMPL_HPP
// infer_localglobal_impl.hpp

#include "gtll/infer_impl.hpp"
#include "gtll/data.hpp"

namespace gll_tool {

class LocalGlobal : public InferenceImpl {
 public:
  Labels infer(const Mat& m, const Gr& gr, const Labels& y);
};

} // namespace

#endif
