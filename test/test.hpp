#ifndef CPP_SEMVER_TEST_HPP
#define CPP_SEMVER_TEST_HPP

#include <iostream>

#define RAISE_TEST_ERROR throw std::runtime_error("Test failed!")

#define TEST_ASSERT( EXP )                       \
  {                                              \
    if ( ! (EXP) )                               \
    {                                            \
      std::cerr << "[FAILED][ASSERT] " << #EXP   \
                << " at line: " << __LINE__      \
                << " in file: " << __FILE__      \
                << std::endl;                    \
      RAISE_TEST_ERROR;                          \
    }                                            \
  }

#define TEST_THROWS( EX, EXP )                 \
  try                                          \
  {                                            \
    EXP;                                       \
    std::cerr << "[FAILED][THROWS] " << #EXP   \
              << " at line: " << __LINE__      \
              << " in file: " << __FILE__      \
              << std::endl;                    \
    RAISE_TEST_ERROR;                          \
  }                                            \
  catch( EX const& ) {                         \
  }                                            \
  catch ( ... )                                \
  {                                            \
    TEST_ASSERT(!"unexpected exception");      \
  }

void run_test();

int main()
{
  
  std::cout << "[Test Suite] " << __FILE__ << std::endl;
  std::cout << std::endl;

  run_test();

  std::cout << std::endl;
  std::cout << "[OK]" << std::endl;

  return 0;
}

#endif