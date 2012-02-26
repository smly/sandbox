#ifndef SELECT_H
#define SELECT_H

#include <string>

class Select {
public:
  Select() { }
  Select(std::string dir): dir_(dir) { }
  void setDir(std::string);
  std::string getDir();
private:
  std::string dir_;
};

#endif
