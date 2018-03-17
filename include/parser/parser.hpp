#ifndef CPP_SEMVER_PARSER_HPP
#define CPP_SEMVER_PARSER_HPP

#include "../type.hpp"

#include <vector>
#include <string>
#include <memory>

namespace semver
{
  const std::string any_space = " \n\r\t\v\f";
  const std::string any_number = "0123456789";
  const std::string any_alphabat = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

  // [    x y    z  ] -> [ x y z ]
  std::string reduce_space(const std::string& str)
  {
    std::string new_string;
    bool pre_space = false;
    for (const char& c : str)
    {
      const bool has_space = (any_space.find(c) != std::string::npos);
      if (pre_space && has_space)
        continue;

      new_string.push_back(c);
      pre_space = has_space;
    }
    return new_string;
  }

  // [    x y    z  ] -> [x y    z]
  std::string trim_string(const std::string& str)
  {
    size_t start = str.find_first_not_of(any_space);
    if (start == std::string::npos)
      return {};

    size_t end = str.find_last_not_of(any_space);
    if (end == std::string::npos)
      return str.substr(start);

    return str.substr(start, end - start + 1);
  }

  std::vector<std::string> split(const std::string& input, const std::string& delimiter, bool trim = false)
  {
    std::vector<std::string> result;

    size_t search_pos = 0;
    while (1)
    {
      size_t found = input.find(delimiter, search_pos);
      if (found == std::string::npos)
      {
        result.emplace_back(input.substr(search_pos));
        break;
      }
      result.emplace_back(input.substr(search_pos, found - search_pos));
      search_pos = found + delimiter.length();
    }

    if (trim)
      for (std::string& s : result)
        s = trim_string(s);

    return result;
  }

  int parse_nr(const std::string& input)
  {
    // nr ::= '0' | ['1'-'9'] ( ['0'-'9'] ) *

    if (input.length() == 0)
      throw semver_error("empty string as invalid number");

    if (input.find_first_not_of(any_number) != std::string::npos)
      throw semver_error("unexpected char as invalid number: '" + input + "'");

    if (input.length() > 1 && input.at(0) == '0')
      throw semver_error("unexpected '0' as invalid number: '" + input + "'");

    try
    {
      return std::stoi(input);
    }
    catch (std::invalid_argument&)
    {
      throw semver_error("invalid number: '" + input + "'");
    }
  }

  std::unique_ptr< int > parse_xr(const std::string& input)
  {
    // xr ::=
    //        'x' | 'X' | '*' |
    if (input == "x" || input == "X" || input == "*")
      return nullptr;

    //        nr
    return std::unique_ptr< int >(new int(parse_nr(input)));
  }

  std::string parse_part(const std::string& input)
  {
    // part  ::=

    if (input.empty() || any_number.find(input.at(0)) != std::string::npos)
    { //         nr |
      parse_nr(input);
    }
    else if (input.find_first_not_of("-" + any_number + any_alphabat) != std::string::npos)
    {
      //         [-0-9A-Za-z]+
      throw semver_error("unexpected character in part: '" + input + "'");
    }

    return input;
  }

  std::string parse_parts(const std::string& input)
  {
    // parts ::= part ( '.' part ) *

    std::vector<std::string> part_tokens = split(input, ".");
    for (const std::string& part_token : part_tokens)
      parse_part(part_token);

    return input;
  }

  syntax::simple parse_partial(const std::string& input)
  {
    // partial ::= xr ( '.' xr ( '.' xr ( '-' pre )? ( '+' build )? ? )? )?
    // pre     ::= parts
    // build   ::= parts

    size_t found_pre = std::string::npos;
    const size_t found_build = input.find_first_of('+');
    const size_t found_dash = input.find_first_of('-');

    if (found_build != std::string::npos && found_dash != std::string::npos)
    {
      if (found_dash < found_build)
        found_pre = found_dash;
    }
    else if (found_build == std::string::npos && found_dash != std::string::npos)
    {
      found_pre = found_dash;
    }

    syntax::simple result;

    {
      if (found_build != std::string::npos)
        result.build = parse_parts(input.substr(found_build + 1));

      if (found_pre != std::string::npos)
      {
        if (found_build != std::string::npos)
          result.pre = parse_parts(input.substr(found_pre + 1, found_build - found_pre - 1));
        else
          result.pre = parse_parts(input.substr(found_pre + 1));
      }
    }

    {
      size_t xr_end = found_pre;
      if (xr_end == std::string::npos && found_build != std::string::npos)
        xr_end = found_build;

      const std::string xr_xr_xr = (xr_end == std::string::npos) ? input : input.substr(0, xr_end);

      std::vector<std::string> xr_tokens = split(xr_xr_xr, ".");
      if ((!result.pre.empty() || !result.build.empty()) && xr_tokens.size() != 3)
        throw semver_error("incomplete version with pre or build tag: '" + xr_xr_xr + "'");

      if (xr_tokens.size() > 0)
      {
        // allow 'v' prefix
        std::string xr = xr_tokens.at(0);
        xr = (xr.find_first_of("vV") == 0) ? xr.substr(1) : xr;
        result.major = std::move(parse_xr(xr));
      }
      if (xr_tokens.size() > 1)
        result.minor = std::move(parse_xr(xr_tokens.at(1)));
      if (xr_tokens.size() > 2)
        result.patch = std::move(parse_xr(xr_tokens.at(2)));
      if (xr_tokens.size() > 3)
        throw semver_error("invalid version: '" + xr_xr_xr + "'");
    }

    return result;
  }

  syntax::simple parse_simple(const std::string& input)
  {
    // simple ::= primitive | partial | tilde | caret
    // primitive  ::= ( '<' | '>' | '>=' | '<=' | '=' | ) partial
    // tilde      ::= '~' partial
    // caret      ::= '^' partial

    const size_t partial_start = input.find_first_not_of("<>=~^");

    if (partial_start == std::string::npos)
      throw semver_error("invalid version: '" + input + "'");

    const std::string prefix = input.substr(0, partial_start);
    syntax::simple result = parse_partial(input.substr(partial_start));

    if (prefix == "=" || prefix.empty())
      result.cmp = syntax::comparator::eq;
    else if (prefix == "<")
      result.cmp = syntax::comparator::lt;
    else if (prefix == ">")
      result.cmp = syntax::comparator::gt;
    else if (prefix == "<=")
      result.cmp = syntax::comparator::lte;
    else if (prefix == ">=")
      result.cmp = syntax::comparator::gte;
    else if (prefix == "~")
      result.cmp = syntax::comparator::tilde;
    else if (prefix == "^")
      result.cmp = syntax::comparator::caret;
    else
      throw semver_error("invalid operator: '" + prefix + "'");

    return result;
  }

  syntax::range parse_range(const std::string& input)
  {
    // range ::=
    std::vector<std::string> hyphen_tokens = split(input, " - ", true);
    if (hyphen_tokens.size() == 2)
    {
      //          hyphen |
      syntax::range hyphen;
      syntax::simple from = parse_partial(hyphen_tokens.at(0));
      syntax::simple to = parse_partial(hyphen_tokens.at(1));
      from.cmp = syntax::comparator::gte;
      to.cmp = syntax::comparator::lte;
      hyphen.emplace_back(std::move(from));
      hyphen.emplace_back(std::move(to));
      return hyphen;
    }
    else if (input.find_first_not_of(any_space) != std::string::npos)
    {
      //          simple ( ' ' simple ) * |
      std::vector<std::string> simple_tokens = split(reduce_space(input), " ", true);
      syntax::range simples;
      for (const std::string& simple_token : simple_tokens)
        simples.emplace_back(parse_simple(simple_token));
      return simples;
    }
    else
    {
      //          ''
      // the input may be a blank string which is allowed as an implcit *.*.* range
      syntax::range implicit_set;
      implicit_set.emplace_back(syntax::simple());
      return implicit_set;
    }
  }

  syntax::range_set parse_range_set(const std::string& input)
  {
    syntax::range_set result;

    // range_set ::= range ( ( ' ' ) * '||' ( ' ' ) * range ) *
    std::vector<std::string> range_tokens = split(input, "||", true);
    for (const std::string& range_token : range_tokens)
      result.emplace_back(parse_range(range_token));

    return result;
  }

  syntax::range_set parser(const std::string input)
  {
    return parse_range_set(input);
  }

}

#endif
