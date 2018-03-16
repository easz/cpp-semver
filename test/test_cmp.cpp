#include "test.hpp"
#include "cpp-semver.hpp"

#include <tuple>
#include <string>
#include <vector>
#include <iostream>

const std::vector< std::tuple< std::string, std::string > > test_data_gt
{
  { "1.0.0", "0.0.0" },
  { "1.0.0", "1.0.0-rc" },
  { "2.3.5", "2.3.4" }
};

const std::vector< std::tuple< std::string, std::string > > test_data_gtr
{
  { "1.0.0", "<1.0.0" },
  { "6.6.6", ">1.0.0 <2.0.0 || 3 - 5" }
};

const std::vector< std::tuple< std::string, std::string > > test_data_ltr
{
  { "0.1.0", ">1.0.0" },
  { "0.6.6", ">1.0.0 <2.0.0 || 3 - 5" }
};

void run_test()
{
  for (const auto& tpl : test_data_gt)
  {
    const auto& v1 = std::get<0>(tpl);
    const auto& v2 = std::get<1>(tpl);

    std::cout << "[Test] gt( \"" << v1 << "\", \"" << v2 << "\" )" << std::endl;
    TEST_ASSERT(semver::gt(v1, v2));
    std::cout << "[Test] gte( \"" << v1 << "\", \"" << v2 << "\" )" << std::endl;
    TEST_ASSERT(semver::gte(v1, v2));

    std::cout << "[Test] lt( \"" << v2 << "\", \"" << v1 << "\" )" << std::endl;
    TEST_ASSERT(semver::lt(v2, v1));
    std::cout << "[Test] lte( \"" << v2 << "\", \"" << v1 << "\" )" << std::endl;
    TEST_ASSERT(semver::lte(v2, v1));

    std::cout << "[Test] !eq( \"" << v1 << "\", \"" << v2 << "\" )" << std::endl;
    TEST_ASSERT(!semver::eq(v1, v2));
    std::cout << "[Test] !eq( \"" << v2 << "\", \"" << v1 << "\" )" << std::endl;
    TEST_ASSERT(!semver::eq(v2, v1));
    std::cout << "[Test] neq( \"" << v1 << "\", \"" << v2 << "\" )" << std::endl;
    TEST_ASSERT(semver::neq(v1, v2));
    std::cout << "[Test] neq( \"" << v2 << "\", \"" << v1 << "\" )" << std::endl;
    TEST_ASSERT(semver::neq(v2, v1));

    std::cout << "[Test] !lt( \"" << v1 << "\", \"" << v2 << "\" )" << std::endl;
    TEST_ASSERT(!semver::lt(v1, v2));
    std::cout << "[Test] !lte( \"" << v1 << "\", \"" << v2 << "\" )" << std::endl;
    TEST_ASSERT(!semver::lte(v1, v2));
  }

  {
    const std::string v1 = "1.2.3-rc";
    const std::string v2 = v1;

    std::cout << "[Test] !gt( \"" << v1 << "\", \"" << v2 << "\" )" << std::endl;
    TEST_ASSERT(!semver::gt(v1, v2));
    std::cout << "[Test] gte( \"" << v1 << "\", \"" << v2 << "\" )" << std::endl;
    TEST_ASSERT(semver::gte(v1, v2));

    std::cout << "[Test] !lt( \"" << v2 << "\", \"" << v1 << "\" )" << std::endl;
    TEST_ASSERT(!semver::lt(v2, v1));
    std::cout << "[Test] lte( \"" << v2 << "\", \"" << v1 << "\" )" << std::endl;
    TEST_ASSERT(semver::lte(v2, v1));

    std::cout << "[Test] eq( \"" << v1 << "\", \"" << v2 << "\" )" << std::endl;
    TEST_ASSERT(semver::eq(v1, v2));
    std::cout << "[Test] !neq( \"" << v1 << "\", \"" << v2 << "\" )" << std::endl;
    TEST_ASSERT(!semver::neq(v1, v2));
  }

  for (const auto& tpl : test_data_gtr)
  {
    const auto& v = std::get<0>(tpl);
    const auto& r = std::get<1>(tpl);

    std::cout << "[Test] gtr( \"" << v << "\", \"" << r << "\" )" << std::endl;
    TEST_ASSERT(semver::gtr(v, r));
  }

  for (const auto& tpl : test_data_ltr)
  {
    const auto& v = std::get<0>(tpl);
    const auto& r = std::get<1>(tpl);

    std::cout << "[Test] ltr( \"" << v << "\", \"" << r << "\" )" << std::endl;
    TEST_ASSERT(semver::ltr(v, r));
  }
}