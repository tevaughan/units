/// @file       test/rational-base-test.cpp
/// @brief      Test-cases for vnix::rat::rational_base.
/// @copyright  2019  Thomas E. Vaughan
/// @license    GPL, Version 3 or later.

#include "../vnix/rat/rational-base.hpp"
#include "catch.hpp"

TEST_CASE("Constants are computed correctly.", "[rational-base]") {
  using rb = vnix::rat::rational_base<5, 3>;
  REQUIRE(rb::BITS == 8);
  REQUIRE(rb::DNM_MASK == 0x07);
  REQUIRE(rb::NMR_MASK == 0xF8);
}
