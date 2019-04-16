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


TEST_CASE("dimval's comparisons work.", "[dimval]") {
  dyndim      ddv1 = sqrt(m / s);
  dyndim      ddv2 = 2 * s;
  auto        sdv1 = ddv1;
  units::time sdv2 = ddv2;

  REQUIRE(ddv1 == ddv1);
  REQUIRE(ddv1 == sdv1);

  REQUIRE(ddv1 != 1.1 * ddv1);
  REQUIRE(ddv1 != 1.1 * sdv1);

  REQUIRE(ddv1 < 1.1 * ddv1);
  REQUIRE(ddv1 < 1.1 * sdv1);

  REQUIRE(ddv1 <= 1.1 * ddv1);
  REQUIRE(ddv1 <= 1.1 * sdv1);

  REQUIRE(ddv1 > 0.9 * ddv1);
  REQUIRE(ddv1 > 0.9 * sdv1);

  REQUIRE(ddv1 >= 0.9 * ddv1);
  REQUIRE(ddv1 >= 0.9 * sdv1);

  REQUIRE_THROWS(ddv1 == ddv2);
  REQUIRE_THROWS(ddv1 == sdv2);

  REQUIRE_THROWS(ddv1 != ddv2);
  REQUIRE_THROWS(ddv1 != sdv2);

  REQUIRE_THROWS(ddv1 < ddv2);
  REQUIRE_THROWS(ddv1 < sdv2);

  REQUIRE_THROWS(ddv1 <= ddv2);
  REQUIRE_THROWS(ddv1 <= sdv2);

  REQUIRE_THROWS(ddv1 > ddv2);
  REQUIRE_THROWS(ddv1 > sdv2);

  REQUIRE_THROWS(ddv1 >= ddv2);
  REQUIRE_THROWS(ddv1 >= sdv2);
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

  REQUIRE(ddv1 - ddv1 == 0 * ddv1);
  REQUIRE(ddv1 - sdv1 == 0 * ddv1);
  REQUIRE(sdv1 - ddv1 == 0 * ddv1);
  REQUIRE(sdv1 - sdv1 == 0 * ddv1);

  REQUIRE_THROWS(ddv1 + ddv2);
  REQUIRE_THROWS(ddv1 + sdv2);
  REQUIRE_THROWS(sdv1 + ddv2);
  REQUIRE_THROWS(sdv1 + sdv2);

  REQUIRE_THROWS(ddv1 - ddv2);
  REQUIRE_THROWS(ddv1 - sdv2);
  REQUIRE_THROWS(sdv1 - ddv2);
  REQUIRE_THROWS(sdv1 - sdv2);

  REQUIRE((ddv1 += sdv1) == 2 * sdv1);
  REQUIRE((ddv1 -= sdv1) == sdv1);
  REQUIRE((sdv1 += ddv1) == 2 * ddv1);
  REQUIRE((sdv1 -= ddv1) == ddv1);

  REQUIRE((ddv1 += dyndim(sdv1)) == 2 * sdv1);
  REQUIRE((ddv1 -= dyndim(sdv1)) == sdv1);
  REQUIRE((sdv2 += units::time(ddv2)) == 2 * ddv2);
  REQUIRE((sdv2 -= units::time(ddv2)) == ddv2);

  REQUIRE_THROWS(ddv1 += ddv2);
  REQUIRE_THROWS(ddv1 += sdv2);
  REQUIRE_THROWS(sdv1 += ddv2);
  REQUIRE_THROWS(sdv1 += sdv2);

  REQUIRE_THROWS(ddv1 -= ddv2);
  REQUIRE_THROWS(ddv1 -= sdv2);
  REQUIRE_THROWS(sdv1 -= ddv2);
  REQUIRE_THROWS(sdv1 -= sdv2);
}


TEST_CASE("dimval's multiplication and division work.", "[dimval]") {
  dyndim ddv1 = 3 * m;
  dyndim ddv2 = 2 * N;
  length sdv1 = ddv1;
  force  sdv2 = ddv2;

  REQUIRE(ddv1 * ddv2 == 6 * J);
  REQUIRE(ddv1 * sdv2 == 6 * J);
  REQUIRE(ddv1 / ddv2 == 1.5 * m / N);
  REQUIRE(ddv1 / sdv2 == 1.5 * m / N);

  REQUIRE(sdv1 * ddv2 == 6 * J);
  REQUIRE(sdv1 * sdv2 == 6 * J);
  REQUIRE(sdv1 / ddv2 == 1.5 * m / N);
  REQUIRE(sdv1 / sdv2 == 1.5 * m / N);

  REQUIRE(ddv1 / ddv1 == 1);
  REQUIRE(ddv1 / sdv1 == 1);
  REQUIRE(sdv1 / sdv1 == 1);
  REQUIRE(sdv1 / ddv1 == 1);

  REQUIRE(ddv2 * 0.5 / N == 1);
  REQUIRE(sdv2 * 0.5 / N == 1);

  REQUIRE((ddv1 *= 2) == 6 * m);
  REQUIRE((ddv1 /= 2) == 3 * m);

  REQUIRE((sdv1 *= 2) == 6 * m);
  REQUIRE((sdv1 /= 2) == 3 * m);
}
