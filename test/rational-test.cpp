#include "../include/vnix/rational.hpp"
#include "catch.hpp"

using namespace vnix;

TEST_CASE("Default args to constructor work as expected.", "[rational]") {
  rat8_t r1;
  REQUIRE(!r1);
  REQUIRE(r1.to_int() == 0);
  rat8_t r2(3);
  REQUIRE(r2);
  REQUIRE(r2.to_int() == 3);
}
