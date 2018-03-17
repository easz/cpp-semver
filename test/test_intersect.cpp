#include "test.hpp"
#include "cpp-semver.hpp"

#include <tuple>
#include <string>
#include <vector>
#include <iostream>


#define TUPL std::make_tuple
#define VECS std::vector<std::string>

const std::vector<
                   std::tuple< std::string,              /* version */
                               std::vector<std::string>, /* intersected */
                               std::vector<std::string>  /* not intersected */> > test_data
{
  TUPL(
    ">=1.2.7",
    VECS{ "1.2.7", "1.2.8", "2.5.3", "1.3.9" },
    VECS{ "1.2.6", "1.1.0", "1.2.7-pre", "6.6.6-pre" }
  ),
  TUPL(
    ">=1.2.7 <1.3.0",
    VECS{ "1.2.7", "1.2.8", "1.2.99" },
    VECS{ "1.2.6", "1.3.0", "1.1.0", "1.2.7-pre", "1.2.9-pre" }
  ),
  TUPL(
    "1.2.7 || >=1.2.9 <2.0.0",
    VECS{ "1.2.7", "1.2.9", "1.4.6", "1.2.9" },
    VECS{ "1.2.8", "2.0.0", "2.*", "1.2.9-pre", "1.3.0-pre" }
  ),
  TUPL(
    ">1.2.3-alpha.3",
    VECS{ "1.2.3-alpha.7", "1.2.3", "2.0.0" },
    VECS{ "1.0.0", "1.2.3-alpha.1", "3.4.5-alpha.9" }
  ),
  TUPL(
    "<1.2.3-alpha.3",
    VECS{ "1.2.3-alpha.1", "1.2.1" },
    VECS{ "1.2.3", "1.1.5-alpha.1", "1.2.3-alpha.4" }
  ),
  TUPL(
    "1.2.3 - 2.3.4",
    VECS{ "1.2.3", "2.3.4", "1.*", "2.3.*" },
    VECS{ "1.0.0", "2.5.*" }
  ),
  TUPL(
    "1.2 - 2.3.4",
    VECS{ "1.2.0", "1.2.*", "2.3.4" },
    VECS{ "1.0.0", "2.5.*" }
  ),
  TUPL(
    "1.2.3 - 2.3",
    VECS{ "1.2.*", "2.3.9999" },
    VECS{ "2.4", "1.2.2", "0 - 1.1.0" }
  ),
  TUPL(
    "  1.2.3   -    2.3  ",
    VECS{ "1.2.*", "2.3.9999" },
    VECS{ "2.4", "1.2.2", "0 -   1.1.0  " }
  ),
  TUPL(
    "1.2.3 - 2",
    VECS{ "1.2.3", "2.999.999" },
    VECS{ "3.0.0", "<1.2.3" }
  ),
  TUPL(
    "1.X",
    VECS{ "1.0.0", "1.*.*" },
    VECS{ "2.0.0" }
  ),
  TUPL(
    "1.2.x",
    VECS{ "1.2.0", "1.2.99" },
    VECS{ "1.3.0" }
  ),
  TUPL(
    "1.2.*",
    VECS{ "1.2", "1.*" },
    VECS{ "1.3.0" }
  ),
  TUPL(
    "*",
    VECS{ "*.*.*", "0.0.0" },
    VECS{ "1.1.1-pre" }
  ),
  TUPL(
    "",
    VECS{ "*.*.*", "0.0.0" },
    VECS{ "1.1.1-pre" }
  ),
  TUPL(
    "1",
    VECS{ "1.0.0" },
    VECS{ "2.0.0" }
  ),
  TUPL(
    "1.2",
    VECS{ "1.2.0" },
    VECS{ "1.3.0" }
  ),
  TUPL(
    "~1.2.3",
    VECS{ ">=1.2.3 <1.3.0", "1.2.3" },
    VECS{ "1.3.0" }
  ),
  TUPL(
    "~1.2",
    VECS{ ">=1.2.0 <1.3.0", "1.2.3", "1.2.x" },
    VECS{ "1.3.0" }
  ),
  TUPL(
    "~1",
    VECS{ "1.x", ">=1.0.0 <2.0.0" },
    VECS{ "2.0.0" }
  ),
  TUPL(
    "~0.2.3",
    VECS{ ">=0.2.3 <0.3.0", "0.2.3" },
    VECS{ "0.3.0" }
  ),
  TUPL(
    "~0.2",
    VECS{ ">=0.2.0 <0.3.0", "0.2.x" },
    VECS{ "0.3" }
  ),
  TUPL(
    "~0",
    VECS{ ">=0.0.0 <1.0.0", "0.x" },
    VECS{ "1.0.0" }
  ),
  TUPL(
    "~1.2.3-beta.2",
    VECS{ ">=1.2.3-beta.2 <1.3.0", "1.2.3-beta.4" },
    VECS{ "1.2.3-alpha", "1.3.0", "1.2.4-beta.2" }
  ),
  TUPL(
    "^1.2.3",
    VECS{ "1.2.3", ">=1.2.3 <2.0.0" },
    VECS{ "2.0.0", "1.2.4-rx" }
  ),
  TUPL(
    "^0.2.3",
    VECS{ ">=0.2.3 <0.3.0" },
    VECS{ "0.3.0" }
  ),
  TUPL(
    "^0.0.3",
    VECS{ ">=0.0.3 <0.0.4" },
    VECS{ "0.0.4" }
  ),
  TUPL(
    "^1.2.3-beta.2",
    VECS{ ">=1.2.3-beta.2 <2.0.0", "1.2.3-beta.4", "1.2.3" },
    VECS{ "1.2.4-beta.2", "2.*" }
  ),
  TUPL(
    "^0.0.3-beta",
    VECS{ ">=0.0.3-beta <0.0.4", "0.0.3-pr.2", "0.0.3" },
    VECS{ "0.1.3-pr.2", "0.0.4" }
  ),
  TUPL(
    "^1.2.x",
    VECS{ "1.2.0", "1.99.11", ">=1.2.0 <2.0.0" },
    VECS{ "2.0.0" }
  ),
  TUPL(
    "^0.0.x",
    VECS{ "0.0.0", "0.0.11", ">=0.0.0 <0.1.0" },
    VECS{ "0.1.0" }
  ),
  TUPL(
    "^0.0",
    VECS{ "0.0.0", "0.0.11", ">=0.0.0 <0.1.0" },
    VECS{ "0.1.0" }
  ),
  TUPL(
    "^1.x",
    VECS{ "1.0.0", ">=1.0.0 <2.0.0" },
    VECS{ "1.0.0-pre", "2.0.0" }
  ),
  TUPL(
    "^0.x",
    VECS{ "0.0.0", "0.99.99", ">=0.0.0 <1.0.0" },
    VECS{ "1.0.0", "0.0.1-pre" }
  ),
  TUPL(
    "=v1.2.3",
    VECS{ "1.2.3" },
    VECS{}
  ),
  TUPL(
    " || || ",
    VECS{ "6.6.6" },
    VECS{ "1.1.1-pre" }
  ),
  TUPL(
    " * || || *.*",
    VECS{ "6.6.6" },
    VECS{ "1.1.1-pre" }
  ),
  TUPL(
    "1.2.3-beta.1.a.3+build1",
    VECS{ "1.2.3-beta.1.a.3", "1.2.3-beta.1.a.3+build110" },
    VECS{ "1.2.3-beta.2", "1.2.*" }
  ),
  TUPL(
    "  0.0.1||  1.2.*  ||2.3.4  ||3.4.5|| 7.8.9 ",
    VECS{ "1.2.9", "7.8.9" },
    VECS{ "1.1.*" }
  ),
  TUPL(
    " 1.0.0 - 2.0.0 || >=3.3.3 <6.6.6 || >=1.2.6-beta.2 ",
    VECS{ "1.0.0", "2.0.0", "1.1.*", "3.3.4", "6.6.6", "1.2.6-rc.2" },
    VECS{ "0.2.5", "6.6.6-rc.2" }
  ),
  TUPL(
    "1.*.*",
    VECS{ "1.0.0", "1.1.0" },
    VECS{ "0.1.0", "2.0.0", "0.*", "2.*", "1.0.0-rc" }
  ),
  TUPL(
    "1.0.*-alpha",
    VECS{ "1.0.0", "1.0.1" },
    VECS{ "0.1.0", "2.0.0", "1.0.1-alpha" }
  ),
  TUPL(
    "1.5.0-a - 2.0.0-r",
    VECS{ "1.5.0", "1.5.0-b", "2.0.0-a", "1.5.5" },
    VECS{ "1.8.9-a", "2.0.0-z", "1.5.5-a" }
  ),
  TUPL(
    "<1.5.*-rc",
    VECS{ "1.4.999" },
    VECS{ "1.5.0", "1.5.0-alpha", "1.0.0-alpha" }
  ),
  TUPL(
    "<2.*.*-rc",
    VECS{ "1.4.9" },
    VECS{ "2.0.0", "2.0.0-alpha", "1.0.0-alpha" }
  ),
  TUPL(
    "<=2.0.*-rc",
    VECS{ "1.9.9", "2.0.0", "2.0.2", },
    VECS{ "2.0.0-alpha", "1.0.0-alpha" }
  ),
  TUPL(
    "<=2.*.*-rc",
    VECS{ "1.9.9", "2.0.0", "2.9.9" },
    VECS{ "2.0.0-alpha", "1.0.0-alpha" }
  ),
  TUPL(
    ">1.3.*-rc",
    VECS{ "1.4.0" },
    VECS{ "1.3.0", "1.3.88", "1.3.0-rc1", "1.4.0-rc1" }
  ),
  TUPL(
    ">1.*.*-rc",
    VECS{ "2.0.0" },
    VECS{ "1.10.0", "1.0.0-rc1", "2.0.0-rc1" }
  ),
  TUPL(
    ">=2.0.*-rc",
    VECS{ "2.0.0", "2.0.2", },
    VECS{ "2.0.0-rc1", "3.0.0-rc1" }
  ),
  TUPL(
    ">=2.*.*-rc",
    VECS{ "2.0.0", "2.9.9" },
    VECS{ "2.0.0-alpha", "1.0.0-alpha" }
  ),
  TUPL(
    "2.0.*-rc",
    VECS{ "2.0.0", "2.0.9" },
    VECS{ "2.1.0", "2.0.0-rc", "2.0.1-rc" }
  ),
  TUPL(
    "2.*.*-rc",
    VECS{ "2.0.0", "2.9.9" },
    VECS{ "1.1.0", "2.0.0-rc", "2.9.9-rc" }
  ),
  TUPL(
    "~2.0.*-a",
    VECS{ "2.0.0" },
    VECS{ "2.0.0-beta", "2.1.0" }
  ),
  TUPL(
    "~2.*.*-a",
    VECS{ "2.0.0", "2.9.9" },
    VECS{ "2.0.0-beta", "1.1.0", "3.0.0" }
  ),
  TUPL(
    "^1.5.6-a",
    VECS{ ">=1.5.6-a <2.0.0", "1.5.6-beta", "1.5.6", "1.9.9" },
    VECS{ "2.0.0", "1.5.7-b" }
  ),
  TUPL(
    "^1.5.*-a",
    VECS{ "1.5.0", "1.5.6", "1.9.9" },
    VECS{ "1.5.0-b", "1.5.7-b" }
  ),
  TUPL(
    "^1.*.*-a",
    VECS{ "1.0.0", "1.5.6", "1.9.9" },
    VECS{ "1.0.0-b", "1.5.7-b" }
  ),
  TUPL(
    "^0.5.6-a",
    VECS{ ">=0.5.6-a <0.6.0", "0.5.6-beta", "0.5.6", "0.5.8" },
    VECS{ "0.6.0", "0.5.8-b" }
  ),
  TUPL(
    "^0.5.6-a",
    VECS{ ">=0.5.6-a <0.6.0", "0.5.6-beta", "0.5.6", "0.5.8" },
    VECS{ "0.6.0", "0.5.8-b" }
  ),
  TUPL(
    "^0.0.6-a2",
    VECS{ ">=0.0.6-a <0.0.7", "0.0.6-beta", "0.0.6" },
    VECS{ "0.0.7", "0.0.6-a1" }
  ),
  TUPL(
    "*.2.3",
    VECS{ "*" },
    VECS{ }
  ),
  TUPL(
    "1.*.3",
    VECS{ "1.*" },
    VECS{ "0.0.1" }
  ),
  TUPL(
    ">2 <1",
    VECS{  },
    VECS{ "*" }
  ),
  TUPL(
    "",
    VECS{ "1.2.3" },
    VECS{ }
  ),
  TUPL(
    "1.1.1-pre--.--.--.000",
    VECS{ },
    VECS{ }
  )
};

const std::vector<std::string> intersected_comp
{
  "1.0.0", ">=1.0.0", ">=1.0.0 <2.0.0", "<1.0.0 || >2.0.0"
};

const std::vector<std::string> not_intersected_comp
{
  "<=1.0.0 >2.0.0", "1.0.0 >2.0.0"
};

void run_test()
{
  for (const std::string& input : intersected_comp)
  {
    std::cout << "[Test] intersects( \"" << input << "\" )" << std::endl;
    TEST_ASSERT(semver::intersects(input));
  }

  for (const std::string& input : not_intersected_comp)
  {
    std::cout << "[Test] !intersects( \"" << input << "\" )" << std::endl;
    TEST_ASSERT(!semver::intersects(input));
  }

  for (const auto& tpl : test_data)
  {
    const auto& input = std::get<0>(tpl);
    const auto& intersected = std::get<1>(tpl);
    const auto& not_intersected = std::get<2>(tpl);

    for (const auto& match : intersected)
    {
      std::cout << "[Test] intersects( \"" << input << "\", \"" << match << "\" )" << std::endl;
      TEST_ASSERT(semver::intersects(input, match));

      std::cout << "[Test] intersects( \"" << match << "\", \"" << input << "\" )" << std::endl;
      TEST_ASSERT(semver::intersects(match, input));
    }

    for (const auto& not_match : not_intersected)
    {
      std::cout << "[Test] !intersects( \"" << input << "\", \"" << not_match << "\" )" << std::endl;
      TEST_ASSERT(!semver::intersects(input, not_match));

      std::cout << "[Test] !intersects( \"" << not_match << "\", \"" << input << "\" )" << std::endl;
      TEST_ASSERT(!semver::intersects(not_match, input));
    }

  }
}