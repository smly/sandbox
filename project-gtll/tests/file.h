#ifndef FILE_H
#define FILE_H

#include <string>

namespace gll_tool {

  class File {
    static bool Exists(const std::string& filename);
  };

}

#endif
