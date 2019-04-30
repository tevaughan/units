/// @file       test/dimval-test.cpp
/// @brief      Test-cases for vnix::units::dimval.
/// @copyright  2019  Thomas E. Vaughan
/// @license    GPL, Version 3 or later.

#include "../vnix/units.hpp"
#include "catch.hpp"
#include <sstream> // for ostringstream

using namespace vnix;
using namespace vnix::units;


TEST_CASE("dimval's dimension is accessible.", "[dimval]") {
  using namespace flt;
  dyndim ddv = sqrt(m / s);

  REQUIRE(ddv.d(dim::off::off(0)) == dim::rat(1, 2));
  REQUIRE(ddv.d(dim::off::off(2)) == dim::rat(-1, 2));

  speed sdv = m / s;

  REQUIRE(sdv.d(dim::off::off(0)) == dim::rat(1));
  REQUIRE(sdv.d(dim::off::off(2)) == dim::rat(-1));
}


TEST_CASE("dimval's comparisons work.", "[dimval]") {
  using namespace dbl;
  dyndim    ddv1 = sqrt(m / s);
  auto      sdv1 = ddv1;
  dyndim    ddv2 = 2.0_s;
  dbl::time sdv2 = ddv2;

  REQUIRE(ddv1 == ddv1);
  REQUIRE(ddv1 == sdv1);

  REQUIRE(ddv1 != 1.1 * ddv1);
  REQUIRE(ddv1 != 1.1 * sdv1);

  REQUIRE(ddv1 < 1.1 * ddv1);
  REQUIRE(ddv1 < 1.1 * sdv1);

  REQUIRE(ddv1 <= 1.1 * ddv1);
  REQUIRE(ddv1 <= 1.1 * sdv1);

  REQUIRE(ddv1 > 0.9 * ddv1);
  REQUIRE(ddv1 > 0.9 * sdv1);

  REQUIRE(ddv1 >= 0.9 * ddv1);
  REQUIRE(ddv1 >= 0.9 * sdv1);

  REQUIRE_THROWS(ddv1 == ddv2);
  REQUIRE_THROWS(ddv1 == sdv2);

  REQUIRE_THROWS(ddv1 != ddv2);
  REQUIRE_THROWS(ddv1 != sdv2);

  REQUIRE_THROWS(ddv1 < ddv2);
  REQUIRE_THROWS(ddv1 < sdv2);

  REQUIRE_THROWS(ddv1 <= ddv2);
  REQUIRE_THROWS(ddv1 <= sdv2);

  REQUIRE_THROWS(ddv1 > ddv2);
  REQUIRE_THROWS(ddv1 > sdv2);

  REQUIRE_THROWS(ddv1 >= ddv2);
  REQUIRE_THROWS(ddv1 >= sdv2);
}


TEST_CASE("dimval's addition and subtraction work.", "[dimval]") {
  using namespace ldbl;
  dyndim     ddv1 = sqrt(m / s);
  dyndim     ddv2 = 2 * s;
  auto       sdv1 = ddv1;
  ldbl::time sdv2 = ddv2;

  REQUIRE(ddv1 + ddv1 == 2 * ddv1);
  REQUIRE(ddv1 + sdv1 == 2 * ddv1);
  REQUIRE(sdv1 + ddv1 == 2 * ddv1);
  REQUIRE(sdv1 + sdv1 == 2 * ddv1);

  REQUIRE(ddv1 - ddv1 == 0 * ddv1);
  REQUIRE(ddv1 - sdv1 == 0 * ddv1);
  REQUIRE(sdv1 - ddv1 == 0 * ddv1);
  REQUIRE(sdv1 - sdv1 == 0 * ddv1);

  REQUIRE_THROWS(ddv1 + ddv2);
  REQUIRE_THROWS(ddv1 + sdv2);
  REQUIRE_THROWS(sdv1 + ddv2);
  REQUIRE_THROWS(sdv1 + sdv2);

  REQUIRE_THROWS(ddv1 - ddv2);
  REQUIRE_THROWS(ddv1 - sdv2);
  REQUIRE_THROWS(sdv1 - ddv2);
  REQUIRE_THROWS(sdv1 - sdv2);

  REQUIRE((ddv1 += sdv1) == 2 * sdv1);
  REQUIRE((ddv1 -= sdv1) == sdv1);
  REQUIRE((sdv1 += ddv1) == 2 * ddv1);
  REQUIRE((sdv1 -= ddv1) == ddv1);

  REQUIRE((ddv1 += dyndim(sdv1)) == 2 * sdv1);
  REQUIRE((ddv1 -= dyndim(sdv1)) == sdv1);
  REQUIRE((sdv2 += dbl::time(ddv2)) == 2 * ddv2);
  REQUIRE((sdv2 -= flt::time(ddv2)) == ddv2);

  REQUIRE_THROWS(ddv1 += ddv2);
  REQUIRE_THROWS(ddv1 += sdv2);
  REQUIRE_THROWS(sdv1 += ddv2);
  REQUIRE_THROWS(sdv1 += sdv2);

  REQUIRE_THROWS(ddv1 -= ddv2);
  REQUIRE_THROWS(ddv1 -= sdv2);
  REQUIRE_THROWS(sdv1 -= ddv2);
  REQUIRE_THROWS(sdv1 -= sdv2);
}


TEST_CASE("dimval's multiplication and division work.", "[dimval]") {
  using namespace flt;
  dyndim ddv1 = 3.0_m;
  dyndim ddv2 = 2.0_N;
  length sdv1 = ddv1;
  force  sdv2 = ddv2;

  REQUIRE(ddv1 * ddv2 == 6 * J);
  REQUIRE(ddv1 * sdv2 == 6 * J);
  REQUIRE((ddv1 / ddv2 / m * N).to_number() == Approx(1.5));
  REQUIRE((ddv1 / sdv2 / m * N).to_number() == Approx(1.5));

  REQUIRE(sdv1 * ddv2 == 6 * J);
  REQUIRE(sdv1 * sdv2 == 6 * J);
  REQUIRE((sdv1 / ddv2 / m * N).to_number() == Approx(1.5));
  REQUIRE((sdv1 / sdv2 / m * N).to_number() == Approx(1.5));

  REQUIRE((ddv1 / ddv1).to_number() == 1);
  REQUIRE((ddv1 / sdv1).to_number() == 1);
  REQUIRE((sdv1 / sdv1).to_number() == 1);
  REQUIRE((sdv1 / ddv1).to_number() == 1);

  REQUIRE((ddv2 * 0.5 / N).to_number() == 1);
  REQUIRE((sdv2 / N).to_number() == 2);

  REQUIRE((ddv1 *= 2) == 6 * m);
  REQUIRE((ddv1 /= 2) == 3 * m);

  REQUIRE((sdv1 *= 2) == 6 * m);
  REQUIRE((sdv1 /= 2) == 3 * m);
}


TEST_CASE("pow and sqrt work for dimval.", "[dimval]") {
  using namespace dbl;
  dyndim ddv1 = 3 * m;
  dyndim ddv2 = pow<2>(ddv1);

  using volume = decltype(m * m * m);
  volume sdv1  = pow<3, 2>(ddv2);

  REQUIRE(ddv2 == 9 * m * m);
  REQUIRE(sqrt(ddv2) == ddv1);
  REQUIRE(sdv1 == 27 * m * m * m);
  REQUIRE((pow<2, 3>(sdv1) / ddv2).to_number() == Approx(1));
  REQUIRE((pow(sdv1, dim::rat(2, 3)) / ddv2).to_number() == Approx(1));
  REQUIRE((sqrt(sdv1) / pow<3, 2>(ddv1)).to_number() == Approx(1));
  std::cout << "sqrt(sdv1) = " << sqrt(sdv1) << std::endl;
  std::cout << "pow(ddv1, {2,3}) = " << pow(ddv1, {3, 2}) << std::endl;
  REQUIRE((sqrt(sdv1) / pow(ddv1, {3, 2})).to_number() == Approx(1));
}


TEST_CASE("Printing works for dimval.", "[dimval]") {
  std::ostringstream oss;
  using namespace ldbl;
  oss << sqrt(1.0 / s) * m;
  REQUIRE(oss.str() == "1 m s^[-1/2]");
}


TEST_CASE("Example in 'README.md' works.", "[dimval]") {
  using namespace flt;
  length    d = 3.0_km;
  flt::time t = 4.0_ms;
  std::cout << "d=" << d << " t=" << t << std::endl;
  auto v = d / t;
  std::cout << v << std::endl;
  std::ostringstream oss;
  oss << v;
  REQUIRE(oss.str() == "750000 m s^-1");
}
