/// @file       test/encoding-test.cpp
/// @brief      Test-cases for vnix::rat::encoding.
/// @copyright  2019  Thomas E. Vaughan
/// @license    GPL, Version 3 or later.

#include "../vnix/rat/encoding.hpp"
#include "catch.hpp"

using normalized_pair = vnix::rat::normalized_pair<uint8_t>;
using encoding        = vnix::rat::encoding<uint8_t>;


TEST_CASE("Limiting values are encoded and decoded.", "[encoding]") {
  encoding const e1(normalized_pair(-16, 1));
  encoding const e2(normalized_pair(+15, 1));
  encoding const e3(normalized_pair(+1, 8));

  REQUIRE(e1.n() == -16);
  REQUIRE(e1.d() == 1);

  REQUIRE(e2.n() == +15);
  REQUIRE(e2.d() == 1);

  REQUIRE(e3.n() == +1);
  REQUIRE(e3.d() == 8);
}
