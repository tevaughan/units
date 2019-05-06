/// @file       test/common-denom-test.cpp
/// @brief      Test-case for vnix::common_denom_params.
/// @copyright  2019 Thomas E. Vaughan; all rights reserved.
/// @license    BSD three-clause; see LICENSE.

#include "../vnix/rat.hpp"
#include "catch.hpp"

using namespace vnix::rat;


TEST_CASE("Number of bits is properly clamped.", "[common-denom-params]") {
  rational<36, 28> r1(3,4);
  rational<28, 36> r2(11,12);
  auto cdp = common_denom(r1, r2);
  using CDP = decltype(cdp);
  REQUIRE(CDP::LCD_BITS == 64);
  REQUIRE(CDP::N1_BITS == 64);
  REQUIRE(CDP::N2_BITS == 56);
}

