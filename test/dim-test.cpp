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
  REQUIRE(d[base_off::TIM] == dim::rat(1));
  REQUIRE(d[base_off::LEN] == dim::rat(2));
  REQUIRE(d[base_off::MAS] == dim::rat(3));
  REQUIRE(d[base_off::CHG] == dim::rat(4));
  REQUIRE(d[base_off::TMP] == dim::rat(5));
}


TEST_CASE("Encoding and decoding works as expected.", "[dim]") {
  dim       d({1, 2, 3, 4, 5});
  dim::word e = d;
  REQUIRE(e == 0b010100'010000'001100'001000'000100);
  REQUIRE(dim(e) == d);
}


TEST_CASE("Addition and subtraction work as expected.", "[dim]") {
  using r = dim::rat;
  r   a(0), b(1), c(2), d(3), e(4, 3);
  r   f(1, 4), g(1, 3), h(1, 2), i(1, 1), j(-1, 3);
  dim x(a, b, c, d, e);
  dim y(f, g, h, i, j);
  REQUIRE(b - g == r(2, 3));
  dim z1(a + f, b + g, c + h, d + i, e + j);
  r   m(-1, 4), n(2, 3), o(3, 2), p(2), q(5, 3);
  dim z2(m, n, o, p, q);
  REQUIRE(n.n() == 2);
  REQUIRE(n.d() == 3);
  REQUIRE(dim(m, n, o, p, q) == z2);
  REQUIRE(z2[base_off::TIM] == m);
  REQUIRE(z2[base_off::LEN] == n);
  REQUIRE(z2[base_off::MAS] == o);
  REQUIRE(z2[base_off::CHG] == p);
  REQUIRE(z2[base_off::TMP] == q);
  dim const zp = x + y;
  dim const zm = x - y;
  REQUIRE(zp == z1);
  REQUIRE(zm == z2);
}


TEST_CASE("Multiplication and division work as expected.", "[dim]") {
  dim::rat a(0), b(1), c(2), d(3), e(3, 2);
  dim::rat f(1, 2);
  dim      x({a, b, c, d, e});
  dim      z1({a * f, b * f, c * f, d * f, e * f});
  dim      z2({a / f, b / f, c / f, d / f, e / f});
  REQUIRE(x * f == z1);
  REQUIRE(x / f == z2);
}


TEST_CASE("dim can print to stream.", "[dim]") {
  dim                d1({1, -1, dim::rat(1, 2), 0, dim::rat(-2, 3)});
  dim                d2({0, 0, 0, 2, 0});
  std::ostringstream oss1, oss2;
  oss1 << d1;
  oss2 << d2;
  REQUIRE(oss1.str() == " m^-1 kg^[1/2] s K^[-2/3]");
  REQUIRE(oss2.str() == " C^2");
}
