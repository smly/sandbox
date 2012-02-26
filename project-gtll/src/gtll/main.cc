#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

#include "gll.hpp"

#define PREC_DEFAULT 8
#define HELP " [-p prec]"\
  "-i inputfile"

void scorer(
    std::vector<int> ans,
    std::vector<int> sys,
    std::vector<int> ulab)
{
  size_t total = ulab.size(), corr = 0;
  assert(sys.size() == ans.size());
  for (unsigned int i = 0; i < ulab.size(); i++) {
    size_t uid = ulab[i];
    if (sys[uid] == ans[uid]) corr++;
  }

  double score = (corr != 0) ? corr / static_cast<double>(total) : 0.0;
  std::cout.setf(std::ios::fixed, std::ios::floatfield);
  std::cout << "number of data:      " << sys.size() << std::endl;
  std::cout << "number of unlabeled: " << ulab.size() << std::endl;
  std::cout << "correct prediction:  " << corr << std::endl;
  std::cout << "error rate:          " << std::setprecision(3) << score << std::endl;
  std::cout.unsetf(std::ios::fixed);
}

int main(int argc, char** argv)
{
  std::string input_fn, answer_fn;

  unsigned int prec = PREC_DEFAULT;
  int opt;
  extern char *optarg;
  while ((opt = getopt(argc, argv, "i:a:p:")) != -1) {
    switch (opt) {
      case 'i': input_fn = std::string(optarg); break;
      case 'a': answer_fn = std::string(optarg); break;
      case 'p': prec = atoi(optarg); break;
      default:
        std::cout << argv[0] << HELP << std::endl;
        exit(EXIT_FAILURE);
    }
  }

  std::ifstream ifs(answer_fn.c_str());
  std::vector<int> ans;
  int ans_;
  while (ifs >> ans_) {
    ans.push_back(ans_);
  }

  gll_tool::Config conf;
  conf.gaussianKernelDelta = 0.1;

  gll_tool::gll model;
  gll_tool::Mat m; // weighted matrix (vec<vec<double>>, symm)
  gll_tool::Gr gr; // purning matrix  (vec<dynamic_bitset<>>, symm)
  gll_tool::Labels y; // predicted result
  model.load(input_fn);
  std::vector<int> ulabel(model.getUnlabeledPos());

  //  m  = model.buildMatrix(conf, gll_tool::IPROD);
  m  = model.buildMatrix(conf, gll_tool::GKERN_L2NORM);
  gr = model.buildGraph(gll_tool::KNN, 2, m);
  // LP
  y  = model.infer(gll_tool::LP, m, gr);
  scorer(ans, y, ulabel);
  // LGC
  y  = model.infer(gll_tool::LGC, m, gr);
  scorer(ans, y, ulabel);

  return 0;
}
