#include "test.hpp"
#include "cpp-semver.hpp"

#include <vector>
#include <string>

const std::vector<std::string> syntax_not_ok
{
  "x0.0.1",
  "-1",
  "1.2.3.4",
  ">= 1.2",
  ">=1.2.",
  "|| blah || ",
  " 1.0.0 -- *.0.0 ",
  "1-2 2-3",
  "1-22-3",
};

void run_test()
{
  for (const auto& input : syntax_not_ok)
  {
    std::cout << "[Test] syntax error \"" << input << "\"" << std::endl;
    TEST_THROWS(semver::semver_error, semver::parse(input));
  }
}
