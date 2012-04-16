#include <iostream>
#include <sstream>
#include <cmath>
using namespace std;
int main()
{
  int T; cin >> T;
  for (int now = 1; now <= T; ++now) {
    stringstream ss;
    ss << "Case #" << now << ": ";
    int N, S, p, ans = 0, ans_dec = 0;
    cin >> N >> S >> p;
    for (int i = 0; i < N; ++i) {
      int max_one, tmp;
      cin >> tmp;
      max_one = (tmp == 0) ? 0 : static_cast<int>(ceil(tmp / 3.0));
      if (max_one >= p) ans++;
      if (max_one == p - 1 && max_one != 0) ans_dec++;
    }
    ss << ans + min(ans_dec, S);
    cout << ss.str() << endl;
  }
}
