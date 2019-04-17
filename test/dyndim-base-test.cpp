/// @file       test/dyndim-base-test.cpp
/// @brief      Test-cases for vnix::units::dyndim_base.
/// @copyright  2019  Thomas E. Vaughan
/// @license    GPL, Version 3 or later.

#include "../vnix/units/statdim-base.hpp"
#include "catch.hpp"

using dim                                = vnix::units::dim;
template <uint64_t D> using statdim_base = vnix::units::statdim_base<D>;
using dyndim_base                        = vnix::units::dyndim_base;


TEST_CASE("Check dimension on dyndim_base from dim.", "[dyndim-base]") {
  dim constexpr d({-1, 1, 0, 0, 0});
  dim e({1, 0, 0, 0, 0});
  REQUIRE(dyndim_base(d).d() == d);
  REQUIRE(dyndim_base(e).d() == e);
}


TEST_CASE("Sum & diff require same dimension for dyndim.", "[dyndim-base]") {
  dim constexpr d1({-1, 1, 1, 0, 0});
  dim constexpr d2({+1, 0, 0, 0, 0});

  dyndim_base      ddb(d1);
  statdim_base<d1> sdb1;
  statdim_base<d2> sdb2;

  REQUIRE_NOTHROW(ddb.sum(ddb));
  REQUIRE_NOTHROW(ddb.sum(sdb1));
  REQUIRE_NOTHROW(ddb.diff(ddb));
  REQUIRE_NOTHROW(ddb.diff(sdb1));

  REQUIRE(ddb.sum(ddb).d() == d1);
  REQUIRE(ddb.sum(sdb1).d() == d1);
  REQUIRE(ddb.diff(ddb).d() == d1);
  REQUIRE(ddb.diff(sdb1).d() == d1);

  REQUIRE_THROWS(ddb.sum(sdb2));
  REQUIRE_THROWS(ddb.diff(sdb2));
}


TEST_CASE("Prod & quot change dimension for dyndim.", "[dyndim-base]") {
  dim constexpr d1({-1, +1, +1, 0, 0});
  dim constexpr d2({+1, +0, +0, 0, 0});
  dim constexpr d3({+0, +1, +1, 0, 0});
  dim constexpr d4({-2, +1, +1, 0, 0});
  dim constexpr d5({+1, -1, -1, 0, 0});

  dyndim_base      ddb1(d1), ddb2(d2);
  statdim_base<d2> sdb2;

  REQUIRE(ddb1.prod(ddb2).d() == d3);
  REQUIRE(ddb1.quot(ddb2).d() == d4);
  REQUIRE(ddb1.prod(sdb2).d() == d3);
  REQUIRE(ddb1.quot(sdb2).d() == d4);
  REQUIRE(ddb1.recip().d() == d5);
}


TEST_CASE("Pow and sqrt change dimension for dyndim.", "[dyndim-base]") {
  using rat = vnix::rat8_t;

  dim constexpr d1({-1, +1, +1, 0, 0});
  dim constexpr d2({-2, +2, +2, 0, 0});
  dim constexpr d3({rat(-1, 2), rat(1, 2), rat(1, 2), 0, 0});

  dyndim_base ddb(d1);

  REQUIRE(ddb.pow<2>().d() == d2);
  REQUIRE(ddb.pow(2).d() == d2);
  REQUIRE(ddb.sqrt().d() == d3);
}
