#include "test.hpp"
#include "cpp-semver.hpp"

#include <vector>
#include <string>

const std::vector<std::string> is_not_valid
{
  "a.b.c",
  "x0.0.1",
  "-1",
  "1.2.3.4",
  ">= 1.2",
  ">=1.2.",
  "|| blah || ",
  " 1.0.0 -- *.0.0 ",
  "1-2 || 2-3",
  "1-22-3",
  ">2 <1"
};

const std::vector<std::string> is_valid
{
  "v0.0.1",
  "=1",
  ">=1.2",
  "1 - 2 || 2 - 3"
};

void run_test()
{
  for (const auto& input : is_valid)
  {
    std::cout << "[Test] valid(\"" << input << "\")" << std::endl;
    TEST_ASSERT(semver::valid(input));
  }

  for (const auto& input : is_not_valid)
  {
    std::cout << "[Test] !valid(\"" << input << "\")" << std::endl;
    TEST_ASSERT(!semver::valid(input));
  }
}
