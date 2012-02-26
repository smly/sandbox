#include <iostream>
#include <vector>
#include <boost/foreach.hpp>
void something(const std::vector<int>& v)
{
  using std::cout;
  using std::endl;

  BOOST_FOREACH (int item, v) {
    cout << item << ' ';
  }
  cout << endl;
}

int main()
{
  using std::vector;
  using std::cout;
  using std::endl;
  something( { 1,1,2,3,4 } );
  const vector<int> v = {1,2,2,3,4,5};
  BOOST_FOREACH (int item, v ) {
    cout << item << ' ';
  }
  cout << endl;
}
