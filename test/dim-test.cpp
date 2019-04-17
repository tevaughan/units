/// @file       test/dim-test.cpp
/// @brief      Test-cases for vnix::units::dim.
/// @copyright  2019  Thomas E. Vaughan
/// @license    GPL, Version 3 or later.

#include "../vnix/units/dim.hpp"
#include "catch.hpp"
#include <sstream> // for ostringstream

using namespace vnix;
using namespace vnix::units;


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


TEST_CASE("Addition and subtraction work as expected.", "[dim]") {
  rat8_t a(0), b(1), c(2), d(3), e(4);
  rat8_t f(1, 6), g(1, 5), h(1, 4), i(1, 3), j(1, 2);
  dim    x({a, b, c, d, e});
  dim    y({f, g, h, i, j});
  dim    z1({a + f, b + g, c + h, d + i, e + j});
  dim    z2({a - f, b - g, c - h, d - i, e - j});
  REQUIRE(x + y == z1);
  REQUIRE(x - y == z2);
}


TEST_CASE("Multiplication and division work as expected.", "[dim]") {
  rat8_t a(0), b(1), c(2), d(3), e(4);
  rat8_t f(1, 2);
  dim    x({a, b, c, d, e});
  dim    z1({a * f, b * f, c * f, d * f, e * f});
  dim    z2({a / f, b / f, c / f, d / f, e / f});
  REQUIRE(x * f == z1);
  REQUIRE(x / f == z2);
}


TEST_CASE("dim can print to stream.", "[dim]") {
  dim                d1({1, -1, rat8_t(1, 2), 0, rat8_t(-2, 3)});
  dim                d2({0, 0, 0, 2, 0});
  std::ostringstream oss1, oss2;
  oss1 << d1;
  oss2 << d2;
  REQUIRE(oss1.str() == " m^-1 kg^[1/2] s K^[-2/3]");
  REQUIRE(oss2.str() == " C^2");
}
