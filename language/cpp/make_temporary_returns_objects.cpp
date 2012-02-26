#include <iostream>

class Count {
 public:
  Count(): cnt(0) { }
  const int count() const
  {
    return cnt;
  }

 protected:
  int cnt;
};

class CountA : public Count {
 public:
  CountA operator++ (int) // post increment operator
  {
    cnt += 1;
  }
};

class CountA2 : public Count {
 public:
  CountA2 operator++ (int)
  {
    cnt += 1;
  }
};

class CountB : public Count {
 public:
  CountB& operator++ (int)
  {
    cnt += 1;
  }
};

class CountC : public Count {
 public:
  CountC& operator++ ()
  {
    cnt *= 2;
  }
  CountC& operator++ (int)
  {
    cnt = cnt + 3;
  }
};

int main()
{
  CountA a;
  a++++++++++; // the second ++ works on a temporary obj.
  std::cout << a.count() << std::endl; // 1

  CountA2 a2;
  a2++;
  std::cout << a2.count() << std::endl; // 1

  CountB b;
  b++++++++++;
  std::cout << b.count() << std::endl; // 5

  CountC c, c2, c3;
  ++c++; // not ++(c++), but ++(c++)
  std::cout << c.count() << std::endl; // 6

  ++++c2++++; // (++(++((c2++)++)))
  std::cout << c2.count() << std::endl; // 24
}
