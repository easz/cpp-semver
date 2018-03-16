## cpp-semver - Semantic Versioning in C++

### Features

 * header-only C++11
 * implements semver string parsing
 * implements significant features from [npm semver](https://docs.npmjs.com/misc/semver)
 * optional to use [PEGTL parser](https://github.com/taocpp/PEGTL)

### API handling string representation

   * ```semver::valid( v )```: return true if the given version or range is syntactically and semantically valid.
   * ```semver::intersects( range1, range2="*" )```: return true if the given version ranges or comparators intersect.
   * ```semver::satisfies( version, range )```: return true if the version satisfies the range.
   * ```semver::gt( v1, v2 )```, ```semver::gte( v1, v2 )```,
     ```semver::lt( v1, v2 )```, ```semver::lte( v1, v2 )```,
     ```semver::eq( v1, v2 )```, ```semver::neq( v1, v2 )```,
     ```semver::gtr( version, range )```, ```semver::ltr( version, range )```: various comparators.
   * ```semver::prerelease( version )```, ```semver::major( version )```,
     ```semver::minor( version )```, ```semver::patch( version )```: get version parts if possible.

### Usage & Demo

```c++
// example/demo.cpp

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

Build with zero-dependency:
```
> g++ -std=c++0x -Iinclude example/demo.cpp
```

Or build with PEGTL:
```
> g++ -std=c++0x -D USE_PEGTL -Iinclude -IPEGTL/include example/demo.cpp
```

The result:
```
> ./a.out
"1.0.0 || 1.5 - 3.0" and ">1.1 <2.0" are intersected.
"<1.* >2.2" is not intersected.

```
