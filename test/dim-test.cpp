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
  std::array<dim::rat, dim::NUM_BASES> a;
  for (unsigned i = 0; i < dim::NUM_BASES; ++i) { a[i] = i; }
  dim d(a);
  for (auto i : dim::off::array) { REQUIRE(d[i] == dim::rat(i)); }
}


TEST_CASE("Encoding and decoding works as expected.", "[dim]") {
  std::array<dim::rat, dim::NUM_BASES> a;
  uint64_t                             t = 0;
  for (unsigned i = 0; i < dim::NUM_BASES; ++i) {
    a[i]      = i;
    using rat = dim::rat;
    enum { BITS = rat::BITS };
    t |= ((rat::encode(i) & bit_range<uint64_t>(0, BITS)) << i * BITS);
  }
  dim       d(a);
  dim::word e = d.encode();
  REQUIRE(e == t);
  REQUIRE(dim(e) == d);
}


TEST_CASE("Addition and subtraction work as expected.", "[dim]") {
  std::array<dim::rat, dim::NUM_BASES> a, b, c, d;
  for (unsigned i = 0; i < dim::NUM_BASES; ++i) {
    a[i] = i;
    std::cout << "a[i]=" << a[i] << std::endl;
    b[i] = dim::rat(-1, 2);
    std::cout << "b[i]=" << b[i] << std::endl;
    c[i] = a[i] + b[i];
    std::cout << "c[i]=" << c[i] << std::endl;
    d[i] = a[i] / 2 - b[i];
    std::cout << "d[i]=" << d[i] << std::endl;
  }
  dim x(a), y(b), z1(c), z2(d);
  REQUIRE(x + y == z1);
  REQUIRE(x / 2 - y == z2);
}


TEST_CASE("Multiplication and division work as expected.", "[dim]") {
  std::array<dim::rat, dim::NUM_BASES> a, c, d;
  dim::rat const                       b(-1, 2);
  for (unsigned i = 0; i < dim::NUM_BASES; ++i) {
    a[i] = i;
    c[i] = a[i] * b;
    d[i] = a[i] / b;
  }
  dim x(a), z1(c), z2(d);
  REQUIRE(x * b == z1);
  REQUIRE(x / b == z2);
}

