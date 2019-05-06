/// @file       test/statdim-base-test.cpp
/// @brief      Test-cases for vnix::units::statdim_base.
/// @copyright  2019 Thomas E. Vaughan; all rights reserved.
/// @license    BSD three-clause; see LICENSE.

#include "../vnix/units/statdim-base.hpp"
#include "catch.hpp"

using dim                                = vnix::units::dim;
template <uint64_t D> using statdim_base = vnix::units::statdim_base<D>;
using dyndim_base                        = vnix::units::dyndim_base;


TEST_CASE("Check dimension on statdim_base from dim.", "[statdim-base]") {
  dim constexpr d(-1, 1, 0, 0, 0);
  dim e(1, 0, 0, 0, 0);
  REQUIRE_NOTHROW(statdim_base<d.encode()>(d));
  REQUIRE_THROWS(statdim_base<d.encode()>(e));
}


TEST_CASE("Sum & diff require same dimension for statdim.", "[statdim-base]") {
  dim constexpr d1(-1, 1, 1, 0, 0);
  dim constexpr d2(+1, 0, 0, 0, 0);

  statdim_base<d1.encode()> sdb;
  dyndim_base               ddb1(d1), ddb2(d2);

  REQUIRE_NOTHROW(sdb.sum(sdb));
  REQUIRE_NOTHROW(sdb.sum(ddb1));
  REQUIRE_NOTHROW(sdb.diff(sdb));
  REQUIRE_NOTHROW(sdb.diff(ddb1));

  REQUIRE(sdb.sum(sdb).d() == d1);
  REQUIRE(sdb.sum(ddb1).d() == d1);
  REQUIRE(sdb.diff(sdb).d() == d1);
  REQUIRE(sdb.diff(ddb1).d() == d1);

  REQUIRE_THROWS(sdb.sum(ddb2));
  REQUIRE_THROWS(sdb.diff(ddb2));
}


TEST_CASE("Prod & quot change dimension for statdim.", "[statdim-base]") {
  dim constexpr d1(-1, +1, +1, 0, 0);
  dim constexpr d2(+1, +0, +0, 0, 0);
  dim constexpr d3(+0, +1, +1, 0, 0);
  dim constexpr d4(-2, +1, +1, 0, 0);
  dim constexpr d5(+1, -1, -1, 0, 0);

  statdim_base<d1.encode()> sdb1;
  statdim_base<d2.encode()> sdb2;
  dyndim_base               ddb2(d2);

  REQUIRE(sdb1.prod(sdb2).d() == d3);
  REQUIRE(sdb1.quot(sdb2).d() == d4);
  REQUIRE(sdb1.prod(ddb2).d() == d3);
  REQUIRE(sdb1.quot(ddb2).d() == d4);
  REQUIRE(sdb1.recip().d() == d5);
}


TEST_CASE("Pow and sqrt change dimension for statdim.", "[statdim-base]") {
  dim constexpr d1(-1, +1, +1, 0, 0);
  dim constexpr d2(-2, +2, +2, 0, 0);

  statdim_base<d1.encode()> sdb1;

  REQUIRE(sdb1.pow<2>().d() == d2);
  REQUIRE(sdb1.pow(2).d() == d2);

  dim constexpr d3(dim::rat(-1, 2), dim::rat(1, 2), dim::rat(1, 2), 0, 0);
  REQUIRE(sdb1.sqrt().d() == d3);
}
