## cpp-semver

Semver implemented in C++

 * implements significant features from [npm semver](https://docs.npmjs.com/misc/semver) in header-only C++11
 * uses [PEGTL parser](https://github.com/taocpp/PEGTL)
 * implemented API
   * ```semver::intersects( range1, range2="*" )```: Return true if the given version ranges or comparators intersect.
   * ```semver::satisfies( version, range )```: Return true if the version satisfies the range.
   * ```semver::gt( v1, v2 )```, ```semver::gte( v1, v2 )```,
     ```semver::lt( v1, v2 )```, ```semver::lte( v1, v2 )```,
     ```semver::eq( v1, v2 )```, ```semver::gtr( version, range )```,
     ```semver::ltr( version, range )```: various comparators.

### demo

```c++
#include "cpp-semver.hpp"
#include <iostream>

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
    const std::string comp = "<1.* >2.2";

    const bool intersected = semver::intersects(comp);

    std::cout << "\"" << comp
      << "\" is " << (intersected ? "" : "not ")
      << "intersected." << std::endl;
  }

  return 0;
}
```

