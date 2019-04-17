#include "../vnix/bit-range.hpp"
#include "catch.hpp"

using namespace vnix;

TEST_CASE("Bit can be set.", "[bit]") {
  REQUIRE(bit<uint8_t>(0) == 0x01);
  REQUIRE(bit<uint8_t>(1) == 0x02);
  REQUIRE(bit<uint8_t>(2) == 0x04);
  REQUIRE(bit<uint8_t>(3) == 0x08);
  REQUIRE(bit<uint8_t>(4) == 0x10);
  REQUIRE(bit<uint8_t>(5) == 0x20);
  REQUIRE(bit<uint8_t>(6) == 0x40);
  REQUIRE(bit<uint8_t>(7) == 0x80);
}

TEST_CASE("Bit-range can be set.", "[bit-range]") {
  for (unsigned off1 = 0; off1 < 8; ++off1) {
    for (unsigned off2 = 0; off2 < 8; ++off2) {
      uint8_t truth = 0;
      if (off1 < off2) {
        for (unsigned i = off1; i <= off2; ++i) {
          truth |= (1 << i);
        }
      } else {
        for (unsigned i = off2; i <= off1; ++i) {
          truth |= (1 << i);
        }
      }
      REQUIRE(bit_range<uint8_t>(off1, off2) == truth);
    }
  }
}
