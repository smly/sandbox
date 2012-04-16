#include <iostream>
#include <map>
#include <sstream>
using namespace std;
int main()
{
  map<char, char> m;
  string source("ejp mysljylc kd kxveddknmc re jsicpdrysirbcpc ypc rtcsra dkh wyfrepkym veddknkmkrkcdde kr kd eoya kw aej tysr re ujdr lkgc jvzq");
  string target("our language is impossible to understandthere are twenty six factorial possibilitiesso it is okay if you want to just give upqz");
  for (int i = 0; i < source.size(); ++i)
    m[ source[i] ] = target[i];
  string line;
  int sz, now = 1;
  cin >> sz;
  cin.ignore();
  while (getline(cin, line)) {
    if (line.empty()) break;
    stringstream ss;
    ss << "Case #" << now++ << ": ";
    for (int i = 0; i < line.size(); ++i) {
      ss << m[line[i]];
    }
    cout << ss.str() << endl;
  }
}
