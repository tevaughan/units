/// @file       test/normalized-pair-test.cpp
/// @brief      Test-cases for vnix::rat::normalized_pair.
/// @copyright  2019 Thomas E. Vaughan; all rights reserved.
/// @license    BSD three-clause; see LICENSE.

#include "../vnix/rat/normalized-pair.hpp"
#include "catch.hpp"

using normalized_pair = vnix::rat::normalized_pair<5, 3>;


TEST_CASE("Input fraction has positive denominator.", "[normalized-pair]") {
  normalized_pair const p(3, -2);
  REQUIRE(p.n() == -3);
  REQUIRE(p.d() == +2);
}


TEST_CASE("Input fraction is reduced.", "[normalized-pair]") {
  normalized_pair const p(-4, 6);
  REQUIRE(p.n() == -2);
  REQUIRE(p.d() == +3);
}


TEST_CASE("Throw on division by zero.", "[normalized-pair]") {
  REQUIRE_THROWS(normalized_pair(1, 0));
}


TEST_CASE("Limits are as expected.", "[normalized-pair]") {
  REQUIRE_NOTHROW(normalized_pair(-16, 1));
  REQUIRE_THROWS(normalized_pair(-17, 1));

  REQUIRE_NOTHROW(normalized_pair(+15, 1));
  REQUIRE_THROWS(normalized_pair(+16, 1));

  REQUIRE_NOTHROW(normalized_pair(+1, 8));
  REQUIRE_THROWS(normalized_pair(+1, 9));
}
