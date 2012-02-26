#include "gll.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdlib> // atoi, atof
#include <string>

// gll_classify LP KNN4 IPROD ./news20.mat
// gll_classify LGC MST GKERN --myu=99 -delta=0.1 ./news20.mat

// Config conf;
// Model mode;
// Graph gr, p;
// gr = model.construct(conf, IPROD); // inner product
// Graph :: N x N symmetry adjacency matrix representation
// gr[0] has N-1 elements, gr[1] has N-2 elements, and so on.

// /* loading svmlight dataformat */
// model.load(conf, filename); // weighted vector

// /* graph constructino with sparsify method */
// conf.similarity = IPROD;
// gr = model.graph(conf);

// conf.k = 4;
// conf.sparsify = KNN;
// p = model.sparsify(conf, gr);

// /* inference */
// conf.max_iter = 10000;
// conf.tol = 1e-7;
// conf.Induction = LP;
// model.classify(conf, gr, p);

/*
  // sample program
  typedef int Label;
  Label ans;
  load_ans(ans);

  Config conf;
  Model model;
  Graph gr, p;

  model.load(conf, filename);
  conf.similarity = IPROD;
  gr = model.graph(conf);
  for (int k = 4; k < 30; k++) {
    conf.k = k;
    conf.sparsify = KNN;
    p = model.sparsify(conf, gr);

    conf.max_iter = 10000;
    conf.tol = 1e-7;
    conf.induction = LP;
    std::vector<Label> l = model.classify(conf, gr, p);
    cout << "k=" << k << ", score=" << scorer(l, ans) << endl;
  }

 */

namespace gll_tool {

// todo: error handling
void gll::appendLine(fv &v, int &lab, const std::string &line)
{
  std::istringstream iss(line);
  std::string lab_str;
  iss >> lab_str;
  if (!lab_str.empty() && !isdigit(lab_str[0])) {
    lab = -1;
  }
  else {
    std::istringstream ss(lab_str);
    ss >> lab;
  }

  const char* f_s = iss.str().c_str();
  const size_t len = strlen(f_s);
  unsigned int pos = 0;
  unsigned int elem_num = 0;
  for (unsigned int i = 0; i < len; i++) {
    if (f_s[i] == ':') elem_num++;
  }
  pos = lab_str.size();
  for (unsigned int i = 0; i < elem_num; i++) {
    unsigned int f_id;
    double w;
    while (pos < len && isspace(f_s[pos])) pos++;
    f_id = atoi(f_s + pos);
    if (f_id > dim_) dim_ = f_id;
    while (pos + 1 < len && f_s[pos] != ':') pos++;
    w = atof(f_s + pos + 1);
    while (pos < len && !isspace(f_s[pos])) pos++;
    v.push_back(std::make_pair(f_id, w));
  }

  sort(v.begin(), v.end(), fv_elem_comp()); // sort by feature id
}

int gll::loadLines(const std::vector<std::string>& lines)
{
  labmax_ = dim_ = N_ = 0;
  mat_.clear();
  mat_.resize(N_);

  const unsigned int N = lines.size();
  for (unsigned int i = 0; i < N; i++) {
    const std::string line = lines[i];
    if (line.empty() || line[0] == '#') continue;
    N_++;

    fv v;
    int lab;
    appendLine(v, lab, line);
    if (labmax_ < lab) labmax_ = lab;
    mat_.push_back(std::make_pair(lab, v));
  }

  return 0;
}

int gll::load(const std::string& filename)
{
  // init
  labmax_ = dim_ = N_ = 0;
  mat_.clear();
  mat_.resize(N_);

  std::ifstream ifs(filename.c_str());
  if (!ifs) {
    std::cerr << "cannnot open " << filename << std::endl;
    return -1;
  }

  std::string line;
  while (getline(ifs, line)) {
    if (line.empty() || line[0] == '#') continue;
    N_++; // data num

    fv v;
    int lab;
    appendLine(v, lab, line);
    if (labmax_ < lab) labmax_ = lab;
    mat_.push_back(std::make_pair(lab, v));
    ifs.peek();
  }

  return 0;
}

std::vector<int> gll::getUnlabeledPos(void) const
{
  std::vector<int> sample_u;
  for (unsigned int i = 0; i < N_; i++) {
    if (mat_[i].first == -1)
      sample_u.push_back(i);
  }

  return sample_u;
}

Mat gll::buildMatrix(
    const Config& conf,
    const SimilarImplType type) const
{
  Similar measure(conf, type);
  Mat m = measure.calcMatrix(mat_);
  assert(m.size() == N_);
  return m;
}

Gr gll::buildGraph(\
    const SparsifyImplType type,
    const size_t n,
    const Mat& m)
{
  Sparse sparse(type, n);
  Gr g = sparse.calcSparsify(m, n);
  return g;
}

Labels gll::infer(
    const InferenceAlgorithm type,
    const Mat& m,
    const Gr& gr)
{
  Infer alg(type);
  Labels y(N_);
  for (unsigned int i = 0; i < N_; i++) {
    y[i] = mat_[i].first;
  }
  return alg.infer(m, gr, y);
}

}// namespace
