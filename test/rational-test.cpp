#include "../include/vnix/rational.hpp"
#include "catch.hpp"
#include <sstream> // for ostringstream

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
  REQUIRE(r2.to_double() == 1.5);
  REQUIRE(r1 == r2);
}


TEST_CASE("Addition and subtraction work.", "[rational]") {
  rat8_t r1(3, 2);
  r1 += 1;
  REQUIRE(r1 == rat8_t(5, 2));
  r1 -= 1;
  REQUIRE(r1 == rat8_t(3, 2));
  rat16_t constexpr r2(-3, 4);
  rat16_t constexpr r3(1, 6);
  REQUIRE(r2 + r3 == rat16_t(-7, 12));
  REQUIRE(r2 - r3 == rat16_t(-11, 12));
}


TEST_CASE("Reciprocal works as expected.", "[rational]") {
  rat8_t r1(-3, 2);
  rat8_t r2(2, 3);
  REQUIRE(r1.reciprocal() == -r2);
  REQUIRE_NOTHROW(rat8_t(8).reciprocal());
  REQUIRE_THROWS(rat8_t(9).reciprocal());
}


TEST_CASE("Multiplication and division work.", "[rational]") {
  rat8_t r1(-3, 2);
  rat8_t r2(-1, 4);
  REQUIRE(r1 * r2 == rat8_t(3, 8));
  REQUIRE(r1 / r2 == rat8_t(6));
  r1 *= rat8_t(1, 2);
  REQUIRE(r1 == rat8_t(-3, 4));
  r1 /= rat8_t(2, 3);
  REQUIRE(r1 == rat8_t(-9, 8));
}


TEST_CASE("Encoding and decoding work as expected.", "[rational]") {
  rat8_t r1(-3,4);
  uint8_t code = 0xE8 | 0x03;
  REQUIRE(rat8_t::encode(r1) == code);
  REQUIRE(rat8_t::decode(code) == r1);
}


TEST_CASE("Comparison operators work as expected.", "[rational]") {
  rat8_t r1(1,2);
  rat8_t r2(-3,8);
  REQUIRE(r1 == r1);
  REQUIRE(r1 != r2);
  REQUIRE(r1 >= r1);
  REQUIRE(r1 >= r2);
  REQUIRE(r1 > r2);
  REQUIRE(r2 <= r1);
  REQUIRE(r2 < r1);
}


TEST_CASE("Unary operators work as expected.", "[rational]") {
  rat8_t r1(4, 6);
  rat8_t r2(-2, 3);
  REQUIRE(r1 == +r1);
  REQUIRE(r1 == -r2);
}


TEST_CASE("Stream-output works as expected.", "[rational]") {
  rat8_t r1 = 4;
  rat8_t r2(-6, 8);
  std::ostringstream oss1, oss2;
  oss1 << r1;
  oss2 << r2;
  REQUIRE(oss1.str() == "4");
  REQUIRE(oss2.str() == "[-3/4]");
}
