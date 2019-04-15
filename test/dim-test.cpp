#include "../include/vnix/units/dim.hpp"
#include "catch.hpp"

using namespace vnix;

using dim      = units::dim;
using base_off = units::base_off;


TEST_CASE("dim can be initialized from array.", "[dim]") {
  dim d({1, 2, 3, 4, 5});
  REQUIRE(d[base_off::TIM] == 1);
  REQUIRE(d[base_off::LEN] == 2);
  REQUIRE(d[base_off::MAS] == 3);
  REQUIRE(d[base_off::CHG] == 4);
  REQUIRE(d[base_off::TMP] == 5);
}


TEST_CASE("Encoding and decoding works as expected.", "[dim]") {
  dim      d({1, 2, 3, 4, 5});
  uint64_t e = d;
  REQUIRE(e == 0x0000002820181008);
  REQUIRE(dim(e) == d);
}
