#include "../vnix/rat/rational-base.hpp"
#include "catch.hpp"

TEST_CASE("Constants are computed correctly.", "[rational-base]") {
  using rb = vnix::rat::rational_base<uint8_t>;
  REQUIRE(rb::BITS == 8);
  REQUIRE(rb::DNM_BITS == 3);
  REQUIRE(rb::NMR_BITS == 5);
  REQUIRE(rb::DNM_MASK == 0x07);
  REQUIRE(rb::NMR_MASK == 0xF8);
}
