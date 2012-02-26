#include "sparsify.hpp"

#include "similar.hpp"
#include <map>
#include <algorithm>

namespace gll_tool {

Sparse::Sparse(const SparsifyImplType type, size_t n)
      : type_(type), n_(n)
{
  switchMethod(type_);
}

void Sparse::switchMethod(const SparsifyImplType impl_type)
{
  if (impl_type == KNN) {
    boost::scoped_ptr< SparsifyImpl > tmp(new KneighborImpl());
    impl_.swap(tmp);
  } else if (impl_type == FULL) {
    boost::scoped_ptr< SparsifyImpl > tmp(new FullImpl());
    impl_.swap(tmp);
  }// todo: add else
}


Gr Sparse::calcSparsify(const Mat& m, size_t sp)
{
  return impl_->calcSparsify(m, sp);
}

bool Sparse::isLinked(\
    const Gr& g,
    const unsigned int i,
    const unsigned int j)
{
  const unsigned int n = g.size();
  if (i >= n || j >= n || i == j) return false;
  if (i > j) return Sparse::isLinked(g, j, i);
  // now i < j, so j-i-1 >= 0
  if (g[i].size() <= j-i-1) return false;
  return g[i][j-i-1] == 1;
}

// sparsify_kneighbor.cc
// for knn impl ###################
typedef std::map<int, double> Map;
typedef Map::iterator MIt;
typedef std::vector<MIt> Dict;
typedef struct DecreasingDict {
  bool operator()(MIt lhs, MIt rhs) const {
    return (lhs->second == rhs->second) ? lhs->first < rhs->first
        : lhs->second > rhs->second;
  }
} DecreasingDict;

Gr Sparse::KneighborImpl::calcSparsify(const Mat& m, size_t sp)
{
  const size_t N = m.size();
  if (N == 0) return Gr(0);
  if (N < sp) sp = N - 1; // N node, N-1 neighbors

  // init
  Gr g(N);
  for (unsigned int i = 0; i < N - 1; i++) {
    g[i].resize(N - i - 1);
  }

  // for all node i
  for (unsigned int i = 0; i < N; i++) {
    // N-1 neighbors, N-1 entries in score
    Map score;
    // for all neighbor of i
    for (unsigned int j = 0; j < N; j++) {
      // ignore self-loop
      if (i == j) continue;
      score[j] = Similar::getWeight(m, i, j);
    }
    Dict d;
    for (MIt it = score.begin(); it != score.end(); it++) d.push_back(it);
    std::nth_element(d.begin(), d.begin() + sp, d.end(), DecreasingDict());
    std::vector<int> knn;
    for (unsigned int j = 0; j < sp; j++) {
      unsigned int dst = d[j]->first;
      if (Similar::getWeight(m, i, dst) > 0.0) {
        if (i > dst) g[dst][i - dst - 1] = 1;
        else g[i][dst - i - 1] = 1;
      }
    }
  }

  return g;
}

// sparsify_full.cc
Gr Sparse::FullImpl::calcSparsify(const Mat& m, size_t sp)
{
  const size_t N = m.size();
  if (N == 0) return Gr(0);

  Gr g(N);
  for (unsigned int i = 0; i < N - 1; i++) {
    g[i].resize(N - i - 1);
    for (unsigned int j = 0; j < N - i - 1; j++) {
      if (m[i][j] > 1e-8) g[i][j] = 1;
    }
  }

  return g;
}

} // namespace 
