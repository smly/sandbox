#ifndef SPARSIFY_HPP
#define SPARSIFY_HPP

#include <boost/scoped_ptr.hpp>
#include "gtll/data.hpp"

namespace gll_tool {

typedef enum SparsifyImplType {
  KNN = 0, // k-nearest neighbor
  BM,      // b-matching graph
  MST,     // minimum spanning tree (maximum weight)
  MBDST,   // minimum bounded degree spanning tree (maximum weight)
  FULL,    // fully connected expect to sim(i,j) == 0
} SparsifyImplType;

class Sparse {
  // implementation for various sparsify methods
  class SparsifyImpl;
  class KneighborImpl;
  class FullImpl;
  class MSTImpl;
  class BDMSTImpl;

  size_t n_;
  SparsifyImplType type_;
  boost::scoped_ptr< SparsifyImpl > impl_;

 public:
  Sparse(const SparsifyImplType type, size_t n);
  ~Sparse() {};

  // switch sparsify method
  void switchMethod(const SparsifyImplType impl_type);
  // return purned matrix
  Gr calcSparsify(const Mat& m, size_t sp);
  // is i and j linked in the graph?
  static bool isLinked(const Gr& g,
                       const unsigned int i,
                       const unsigned int j);
};

class Sparse::SparsifyImpl {
 public:
  virtual Gr calcSparsify(const Mat& m, size_t sp) = 0;
 private:
};

// sparsify_kneighbor.hpp
class Sparse::KneighborImpl : public Sparse::SparsifyImpl {
 public:
  Gr calcSparsify(const Mat& m, size_t sp);
};

// sparsify_full.hpp
class Sparse::FullImpl : public Sparse::SparsifyImpl {
 public:
  Gr calcSparsify(const Mat& m, size_t sp);
};

} // gll_tool

#endif
