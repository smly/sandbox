#ifndef INFER_HPP
#define INFER_HPP

#include "gtll/data.hpp"
#include "gtll/infer_impl.hpp"
#include "gtll/infer_labelpropagation_impl.hpp"
#include "gtll/infer_localglobal_impl.hpp"

#include <boost/scoped_ptr.hpp>

namespace gll_tool {

typedef enum InferenceAlgorithm {
  LP = 0, // label propagation
  LGC,    // local and global consisntency
  GTAM,   // graph transduction via alternating minimization
} InferenceAlgorithm;

class InferenceImpl;
class LabelPropagation;
class LocalGlobal;
class AlternatingMin;

class Infer {

  InferenceAlgorithm type_;
  boost::scoped_ptr< InferenceImpl > impl_;

 public:
  Infer(const InferenceAlgorithm type);
  ~Infer() {}

  void switchMethod(const InferenceAlgorithm impl_type);
  // estimate Y_u from X from Y_l
  Labels infer(const Mat& m, const Gr& gr, const Labels& y);
};

}

#endif
