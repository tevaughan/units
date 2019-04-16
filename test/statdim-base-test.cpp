#include "../include/vnix/units/statdim-base.hpp"
#include "catch.hpp"

using dim                                = vnix::units::dim;
template <uint64_t D> using statdim_base = vnix::units::statdim_base<D>;

TEST_CASE("Check dimension on initialization from dim.", "[statdim-base]") {
  dim constexpr d({-1, 1, 0, 0, 0});
  dim e({1, 0, 0, 0, 0});
  REQUIRE_NOTHROW(statdim_base<d>(d));
  REQUIRE_THROWS(statdim_base<d>(e));
}
