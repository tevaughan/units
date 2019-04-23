/// @file       test/encoding-test.cpp
/// @brief      Test-cases for vnix::rat::encoding.
/// @copyright  2019  Thomas E. Vaughan
/// @license    GPL, Version 3 or later.

#include "../vnix/rat/encoding.hpp"
#include "catch.hpp"

using np8_t = vnix::rat::normalized_pair<5, 3>;
using np9_t = vnix::rat::normalized_pair<5, 4>;
using en8_t = vnix::rat::encoding<5, 3>;
using en9_t = vnix::rat::encoding<5, 4>;


TEST_CASE("Constants are computed correctly.", "[encoding]") {
  using en = vnix::rat::encoding<5, 3>;
  REQUIRE(en::BITS == 8);
  REQUIRE(en::DNM_MASK == 0x07);
  REQUIRE(en::NMR_MASK == 0xF8);
}


TEST_CASE("Limiting values are encoded and decoded.", "[encoding]") {
  en8_t const e1(np8_t(-16, 1));
  en8_t const e2(np8_t(+15, 1));
  en8_t const e3(np8_t(+1, 8));
  en9_t const e4(np9_t(-16, 1));
  en9_t const e5(np9_t(+15, 1));
  en9_t const e6(np9_t(+1, 16));

  REQUIRE(e1.n() == -16);
  REQUIRE(e1.d() == 1);

  REQUIRE(e2.n() == +15);
  REQUIRE(e2.d() == 1);

  REQUIRE(e3.n() == +1);
  REQUIRE(e3.d() == 8);

  REQUIRE(e4.n() == -16);
  REQUIRE(e4.d() == 1);

  REQUIRE(e5.n() == +15);
  REQUIRE(e5.d() == 1);

  REQUIRE(e6.n() == +1);
  REQUIRE(e6.d() == 16);
}
