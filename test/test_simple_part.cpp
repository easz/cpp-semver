#include "test.hpp"
#include "cpp-semver.hpp"

#include <tuple>
#include <vector>
#include <string>

const std::vector< std::tuple< 
  std::string,/* input */
  int,/* major */
  int,/* minor */
  int,/* patch */
  std::vector<std::string>/* pre*/ > > test_data
{
  { "1.2.3-pre.4.5.6", 1, 2, 3, { "pre", "4", "5", "6" } },
  { "1.2.3-pre.40.50.60--+build.666", 1, 2, 3,{ "pre", "40", "50", "60--" } },
  { "1.2.3", 1, 2, 3, {} },
  { "1.2", 1, 2, 0, {} },
};

void run_test()
{

  for (const auto& tpl : test_data)
  {
    const auto& input = std::get<0>(tpl);
    const auto& major = std::get<1>(tpl);
    const auto& minor = std::get<2>(tpl);
    const auto& patch = std::get<3>(tpl);
    const auto& pre = std::get<4>(tpl);

    std::cout << "[Test] major(\"" << input << "\")" << std::endl;
    TEST_ASSERT(major == semver::major(input));
    
    std::cout << "[Test] minor(\"" << input << "\")" << std::endl;
    TEST_ASSERT(minor == semver::minor(input));
    
    std::cout << "[Test] patch(\"" << input << "\")" << std::endl;
    TEST_ASSERT(patch == semver::patch(input));
    
    std::cout << "[Test] prerelease(\"" << input << "\")" << std::endl;
    const auto prerelease = semver::prerelease(input);

    TEST_ASSERT(pre.size() == prerelease.size());
    for (size_t i = 0; i < pre.size(); i++)
      TEST_ASSERT(pre.at(i) == prerelease.at(i));
  }

}
