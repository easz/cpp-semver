#include "cpp-semver.hpp"

#include <iostream>

int main()
{
  {
    const std::string ver1 = "1.0.0 || 1.5.6 - 2.5.6";
    const std::string ver2 = ">1.0.0 <2.0.0";

    const bool intersected = semver::intersects(ver1, ver2);

    std::cout << "\"" << ver1
      << "\" and \"" << ver2
      << "\" are " << (intersected ? "" : "not ")
      << "intersected." << std::endl;
  }

  {
    const std::string ver1 = ">=1.* <2.2";
    const std::string ver2 = "2.8";

    const bool intersected = semver::intersects(ver1, ver2);

    std::cout << "\"" << ver1
      << "\" and \"" << ver2
      << "\" are " << (intersected ? "" : "not ")
      << "intersected." << std::endl;
  }

  return 0;
}