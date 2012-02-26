#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace gll_tool {

class File {
 public:
  static bool Exists(const std::string& filename);
};

class Path {
 public:
  static std::string TestSourceDir();
};

}

#endif
