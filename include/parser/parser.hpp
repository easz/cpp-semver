#ifndef CPP_SEMVER_PARSER_HPP
#define CPP_SEMVER_PARSER_HPP

#include "../type.hpp"

#include <memory>
#include <cassert>

namespace semver
{

  semver::syntax::range_set default_parser(const std::string input)
  {
    assert(!"not implemented yet");
    semver::syntax::range_set result;
    return result;
  }

}

#endif