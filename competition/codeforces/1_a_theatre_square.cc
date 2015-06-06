#include <iostream>
using namespace std;
typedef unsigned long long ull;

int main(int argc, char** argv)
{
  ull n, m, a;
  cin >> n >> m >> a;
  cout << (1 + ((n-1)/a)) * (1 + ((m-1)/a)) << endl;
}
