#include "utils.hpp"
#include <iostream>
#include <cstdlib>
#include <cstdio>

namespace gll_tool {

bool File::Exists(const std::string& filename)
{
  return access(filename.c_str(), F_OK) == 0;
}

std::string Path::TestSourceDir()
{
  std::string prefix(getenv("PWD"));
  std::string partial = "/src/testing";

  while (!File::Exists(prefix + partial)) {
    if (!File::Exists(prefix)) {
      return "."; // failure
    }
    prefix += "/..";
  }

  return prefix + "/src";
}

} // gll_tool
