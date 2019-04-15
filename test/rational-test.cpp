#include "../include/vnix/rational.hpp"
#include "catch.hpp"

using namespace vnix;


TEST_CASE("Constructor from num and den works as expected.", "[rational]") {
  rat8_t constexpr r1;
  REQUIRE(!r1.to_bool());
  REQUIRE(r1.to_int() == 0);

  rat8_t constexpr r2(3);
  REQUIRE(r2.to_bool());
  REQUIRE(r2.to_int() == 3);

  rat8_t constexpr r3(3, 2);
  REQUIRE(r3.to_bool());
  REQUIRE_THROWS(r3.to_int());
  REQUIRE(r3.to_double() == 1.5);

  rat8_t constexpr r4(4, -4);
  REQUIRE(r4.to_bool());
  REQUIRE(r4.to_int() == -1);
}


TEST_CASE("Conversion-constructor works as expected.", "[rational]") {
  rat8_t constexpr r1(3, 2);
  rat16_t constexpr r2(r1);
  REQUIRE(r1 == r2);
}
