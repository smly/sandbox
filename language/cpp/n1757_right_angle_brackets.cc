#include <iostream>
#include <vector>
#include <boost/foreach.hpp>
#include <boost/assign/std/vector.hpp>
int main()
{
  using std::vector;
  using std::cout;
  using std::endl;
  using namespace boost::assign;
  vector<int> a, b, c;
  vector<vector<int>> m = { {0,2}, {1,4} };
  a += 1,2,2,3,3,3,4,4,4,4;
  b += 1,2,3,4;
  c += 1234;
  m += a, b, c;
  BOOST_FOREACH (const vector<int> &v, m) {
    cout << v.size() << endl;
  }
  return 0;
}
