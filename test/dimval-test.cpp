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
