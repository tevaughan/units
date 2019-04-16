#include "../include/vnix/unitsf.hpp"
#include "catch.hpp"

using namespace vnix;
using namespace vnix::units;


TEST_CASE("dimval's dimension is accessible.", "[dimval]") {
  dyndim ddv = sqrt(m / s);
  speed  sdv = m / s;
  REQUIRE(ddv.d(base_off::LEN) == rat8_t(1, 2));
  REQUIRE(sdv.d(base_off::LEN) == 1);
  REQUIRE(ddv.d(base_off::TIM) == rat8_t(-1, 2));
  REQUIRE(sdv.d(base_off::TIM) == -1);
}


TEST_CASE("dimval's addition and subtraction work.", "[dimval]") {
  dyndim      ddv1 = sqrt(m / s);
  dyndim      ddv2 = 2 * s;
  auto        sdv1 = ddv1;
  units::time sdv2 = ddv2;
  REQUIRE(ddv1 + ddv1 == 2 * ddv1);
  REQUIRE(ddv1 + sdv1 == 2 * ddv1);
  REQUIRE(sdv1 + ddv1 == 2 * ddv1);
  REQUIRE(sdv1 + sdv1 == 2 * ddv1);
  REQUIRE_THROWS(ddv1 + ddv2);
  REQUIRE_THROWS(ddv1 + sdv2);
  REQUIRE_THROWS(sdv1 + ddv2);
  REQUIRE_THROWS(sdv1 + sdv2);
}
