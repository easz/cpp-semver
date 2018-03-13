#ifndef CPP_SEMVER_HPP
#define CPP_SEMVER_HPP

#include "type.hpp"
#include "peg.hpp" // parsing with PETGTL

#include <string>
#include <vector>
#include <memory>

namespace semver
{
  /** parse version range in string into syntactical representation @c syntax::range_set */
  syntax::range_set parse(const std::string& input)
  {
    return peg_parse(input); // parsing with PETGTL
  }

  /** parse syntactical representation @c syntax::simple and convert it to sementical representation @c semantic::interval */
  semantic::interval parse(const syntax::simple& input)
  {
    // default result * := [min, max]
    semantic::interval result;

    semantic::boundary b; // associate boundary from input x.y.z-pre
                          // replace * with 0
    b.major = (input.major ? *input.major : 0);
    b.minor = (input.minor ? *input.minor : 0);
    b.patch = (input.patch ? *input.patch : 0);
    b.pre = input.pre;

    switch (input.cmp)
    {
    case syntax::comparator::lt:         // <x.y.z-pre := [min, x.y.z-pre)

      result.to_inclusive = false;
      result.to = b;

      if (!input.major || !input.minor || !input.patch)  // <x.*-pre   := [min, x.0.0)
        result.to.pre = "";                              // <x.y.*-pre := [min, x,y.0)

      break;

    case syntax::comparator::lte:        // <=x.y.z.pre := [min, x.y.z-pre]

      result.to = b;

      if (!input.major || !input.minor || !input.patch)  // <=x.*-pre   := [min, 'x+1'.0.0)
      {                                                  // <=x.y.*-pre := [min, x,'y+1'.0)
        result.to_inclusive = false;
        result.to.pre = "";
      }

      if (!input.major)              // <=* := [min, max]
      {
        result = semantic::interval();
      }
      else if (!input.minor)             // <=x.*-pre := [min, 'x+1'.0.0)
      {
        result.to.major += 1;
        result.to.patch = 0;
      }
      else if (!input.patch)             // <=x.y.*-pre := [min, x,'y+1'.0)
      {
        result.to.minor += 1;
      }

      break;

    case syntax::comparator::gt:         // >x.y.z-pre := (x.y.z-pre, max]
      result.from_inclusive = false;
      result.from = b;

      if (!input.major || !input.minor || !input.patch)  // >x.*-pre   := ['x+1', max]
      {                                                  // >x.y.*-pre := [x.'y+1', max]
        result.from_inclusive = true;
        result.from.pre = "";
      }

      if (!input.major)
      {
        result.from = semantic::boundary::max(); // invalid set
      }
      else if (!input.minor)             // >x.*-pre := ['x+1'.0.0, max]
      {
        result.from.major += 1;
        result.from.patch = 0;
      }
      else if (!input.patch)             // >x.y.*-pre := [x,'y+1'.0, max]
      {
        result.from.minor += 1;
      }

      break;

    case syntax::comparator::gte:        // >=x.y.z-pre := [x.y.z-pre, max]

      result.from = b;

      if (!input.major || !input.minor || !input.patch)  // >=x.*-pre   := [x.0.0, max]
        result.from.pre = "";                            // >=x.y.*-pre := [x.y.0, max]

      break;

    case syntax::comparator::caret:

      result.from = b;
      result.to = b;

      if (!input.major || !input.minor || !input.patch)
        result.from.pre = "";

      result.to_inclusive = false;
      result.to.pre = "";

      if (!input.major)              // ^* := [min, max]
      {
        result = semantic::interval();
      }
      else if (*input.major != 0 ||  // ^x.y.z-pre := [x.y.z-pre, 'x+1'.0.0)
               !input.minor)         // ^x.y       := [x.y.0, 'x+1'.0.0)
      {                              // ^x         := [x.0.0, 'x+1'.0.0)
        result.to.major += 1;        // ^0         := [0.0.0, 1.0.0)
        result.to.minor = 0;
        result.to.patch = 0;
      }
      else if (*input.minor != 0 || // ^0.y.z := [0.y.z, 0.'y+1'.z)
               !input.patch)        // ^0.y   := [0.y.0, 0.'y+1'.0)
      {
        result.to.minor += 1;
        result.to.patch = 0;
      }
      else if (*input.patch != 0)
      {
        result.to.patch += 1; // ^0.0.z := [0.0.z, 0.0.'z+1')
      }

      break;

    case syntax::comparator::tilde:

      result.from = b;
      result.to = b;

      if (!input.major || !input.minor || !input.patch)
        result.from.pre = "";

      result.to_inclusive = false;
      result.to.pre = "";

      if (!input.major)              // ~* := [min, max]
      {
        result = semantic::interval();
      }
      else if (!input.minor)         // ~x-pre := [x.0.0, 'x+1'.0.0)
      {
        result.from.pre = "";

        result.to.major += 1;
        result.to.minor = 0;
        result.to.patch = 0;
      }
      else                           // ~x.y.z-pre := [x.y.z-pre, x.'y+1'.0)
      {
        result.to.minor += 1;
        result.to.patch = 0;
      }

      break;

    case syntax::comparator::eq:
    default:

      result.from = b;                   // x.y.z := [x.y.z, x.y.z]
      result.to = b;

      if (!input.major || !input.minor || !input.patch)  // =x.*-pre   := [x.0.0, 'x+1'.0.0)
      {                                                  // =x.y.*-pre := [x.y.0, x.'y+1'.0)
        result.from.pre = "";

        result.to_inclusive = false;
        result.to.pre = "";
      }

      if (!input.major)                  // * := [min, max]
      {
        result = semantic::interval();
      }
      else if (!input.minor)             // x := [x.0.0, 'x+1'.0.0)
      {
        result.from.patch = 0;

        result.to.major += 1;
        result.to.patch = 0;
      }
      else if (!input.patch)              // x.y   := [x.y.0, x.'y+1'.0)
      {
        result.to.minor += 1;
      }
    }

    return result;
  };

  /** calculate AND-conjunction from a list of @c semantic::interval */
  std::unique_ptr<semantic::interval> and_conj(const std::vector< semantic::interval >& input)
  {
    const size_t size = input.size();

    if (size == 0)
      return nullptr;

    semantic::interval result = input.at(0);

    for (size_t i = 1; i < size; i++)
    {
      const auto& span = input.at(i);

      // prerelease tag case
      {
        const bool result_is_range = (result.from != result.to);
        const bool span_is_range = (span.from != span.to);

        if (result_is_range != span_is_range)
        {
          const auto& range = (result_is_range ? result : span);
          const auto& target = (result_is_range ? span : result);
          const bool range_from_has_pre = !range.from.pre.empty();
          const bool range_to_has_pre = !range.to.pre.empty();
          const bool target_has_pre = !target.from.pre.empty();

          if (target_has_pre)
          {
            // 1-rc not in '0 - 10'
            if (!range_from_has_pre && !range_to_has_pre)
              return nullptr;

            // 1-rc in '1-beta - 10'
            if (range_from_has_pre && !range_to_has_pre)
              if (range.from.major != target.from.major ||
                range.from.minor != target.from.minor ||
                range.from.patch != target.from.patch)
                return nullptr;

            // 10-beta in '1 - 10-rc'
            if (!range_from_has_pre && range_to_has_pre)
              if (range.to.major != target.to.major ||
                range.to.minor != target.to.minor ||
                range.to.patch != target.to.patch)
                return nullptr;

            // 1-rc or 10-beta in '1-beta - 10-rc'
            if (range_from_has_pre && range_to_has_pre)
              if (!((range.from.major == target.from.major &&
                range.from.minor == target.from.minor &&
                range.from.patch == target.from.patch) ||
                (range.to.major == target.to.major &&
                  range.to.minor == target.to.minor &&
                  range.to.patch == target.to.patch)))
                return nullptr;
          }
        }
      } // prerelease tag case

      if ((span.from > result.from) ||
        ((span.from == result.from) && !span.from_inclusive))
      {
        result.from = span.from;
        result.from_inclusive = span.from_inclusive;
      }

      if ((span.to < result.to) ||
          ((span.to == result.to) && !span.to_inclusive))
      {
        result.to = span.to;
        result.to_inclusive = span.to_inclusive;
      }
    }

    if ((result.from > result.to) ||
        ((result.from == result.to) && (result.from_inclusive != result.to_inclusive)))
      return nullptr;

    return std::make_unique<semantic::interval>(result);
  }

  /** parse syntactical representation @c syntax::range_set and convert it to sementical representation @c semantic::interval_set */
  semantic::interval_set parse(const syntax::range_set& input)
  {
    semantic::interval_set or_set;
    for (const syntax::range& range : input)
    {
      std::vector<semantic::interval> and_set;
      for (const syntax::simple& simple : range)
        and_set.emplace_back(parse(simple));
      const std::unique_ptr<semantic::interval> conj = and_conj(and_set);
      if (conj)
        or_set.emplace_back(std::move(*conj));
    }
    return or_set;
  }

  /** check if two @c semantic::interval_set intersect with each other */
  bool intersects(const semantic::interval_set& s1, const semantic::interval_set& s2)
  {
    if (s1.empty() || s2.empty())
      return false;

    for (const semantic::interval& intval_1 : s1)
      for (const semantic::interval& intval_2 : s2)
        if (and_conj({ intval_1, intval_2 }))
          return true;

    return false;
  }

  /** check if two @c syntax::range_set intersect with each other */
  bool intersects(const syntax::range_set& rs1, const syntax::range_set& rs2)
  {
    return intersects(parse(rs1), parse(rs2));
  }

  /** check if two version range in string intersect with each other */
  bool intersects(const std::string& str1, const std::string& str2)
  {
    return intersects(parse(str1), parse(str2));
  }

}

#endif