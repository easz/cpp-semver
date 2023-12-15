#include <iostream>

#include "cpp-semver.hpp"

int main()
{
  {
    const std::string ver1 = "1.0.0 || 1.5 - 3.0";
    const std::string ver2 = ">1.1 <2.0";

    const bool intersected = semver::intersects(ver1, ver2);

    std::cout << "\"" << ver1
      << "\" and \"" << ver2
      << "\" are " << (intersected ? "" : "not ")
      << "intersected." << std::endl;
  }

  {
    const std::string comp = "<1.0 >2.2";

    const bool intersected = semver::intersects(comp);

    std::cout << "\"" << comp
      << "\" is " << (intersected ? "" : "not ")
      << "intersected." << std::endl;
  }

  {
    const std::string ver = "1.2.3";
    const std::string range = "1.x || >=2.5.0 || 5.0.0 - 7.2.3";

    const bool satisfied = semver::satisfies(ver, range);

    std::cout << "\"" << ver
      << "\" is " << (satisfied ? "" : "not ")
      << "satisfied by "
      << "\"" << range << "\"." << std::endl;
  }

  return 0;
}