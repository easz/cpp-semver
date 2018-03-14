#include "cpp-semver.hpp"

#include <tuple>
#include <string>
#include <iostream>

#define TEST_ASSERT( EXP )                       \
  {                                              \
    if ( ! (EXP) )                               \
    {                                            \
      std::cerr << "[FAILED ASSERT] " << #EXP    \
                << " at line: " << __LINE__      \
                << std::endl;                    \
      throw std::runtime_error("Test failed!");  \
    }                                            \
  }

#define TEST_THROWS( EX, EXP )                 \
  try                                          \
  {                                            \
    EXP;                                       \
    std::cerr << "[FAILED THROWS] " << #EXP    \
              << " at line: " << __LINE__      \
              << std::endl;                    \
    throw std::runtime_error("Test failed!");  \
                                               \
  }                                            \
  catch( EX const& ) {                         \
  }                                            \
  catch ( ... )                                \
  {                                            \
    TEST_ASSERT(!"unexpected exception");      \
  }


const std::vector<
                   std::tuple< std::string,              /* version */
                               std::vector<std::string>, /* intersected */ 
                               std::vector<std::string>  /* not intersected */> > test_data
{
  {
    ">=1.2.7",
    { "1.2.7", "1.2.8", "2.5.3", "1.3.9" },
    { "1.2.6", "1.1.0", "1.2.7-pre", "6.6.6-pre" }
  },
  {
    ">2 <1",
    {  },
    { "*" }
  },
};

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

void test_syntax()
{
  for (const auto& input : syntax_not_ok)
  {
    std::cout << "[Test] syntax error \"" << input << "\"" << std::endl;
    TEST_THROWS(semver::semver_error, semver::parse(input));
  }
}

void test_intersect()
{
  for (const auto& tpl : test_data)
  {
    const auto& input = std::get<0>(tpl);
    const auto& intersected = std::get<1>(tpl);
    const auto& not_intersected = std::get<2>(tpl);

    const auto v1 = semver::parse(input);
    
    for (const auto& match : intersected)
    {
      std::cout << "[Test] intersects( \"" << input << "\", \"" << match << "\" )" << std::endl;

      const auto v2 = semver::parse(match);
      TEST_ASSERT( semver::intersects(v1, v2) );
      TEST_ASSERT( semver::intersects(v2, v1) );
    }
    
    for (const auto& not_match : not_intersected)
    {
      std::cout << "[Test] !intersects( \"" << input << "\", \"" << not_match << "\" )" << std::endl;

      const auto v2 = semver::parse(not_match);
      TEST_ASSERT( !semver::intersects(v1, v2) );
      TEST_ASSERT( !semver::intersects(v2, v1) );
    }
    
  }
}

int main()
{
  test_syntax();
  test_intersect();

  return 0;
}