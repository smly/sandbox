#ifndef DATA_HPP
#define DATA_HPP

#include <vector>
#include <boost/dynamic_bitset.hpp>

namespace gll_tool {

class Config {
public:
  Config():
      k(10),
      gaussianKernelDelta(1),
      tol(1e-9),
      max_iter(10000)
  { }
  // gaussian kernel bandwidth parameter
  double gaussianKernelDelta;
  size_t k;     // for KNN

  double tol;   // tolerance for the residual error
  size_t max_iter; // maximum allowable number of iteration
};

typedef int label; // unlabeled instance as -1
typedef std::pair<int, double> fv_elem;
typedef std::vector<fv_elem> fv;
typedef std::pair<label, fv> sample;
typedef std::vector<sample> sample_v;

// weighted matrix W
typedef std::vector<std::vector<double> > Mat;
// purning matrix P
typedef boost::dynamic_bitset<> AdjacencyList;
typedef std::vector<AdjacencyList> Gr;

// labels
typedef int Label;
typedef std::vector<Label> Labels;

}

#endif
