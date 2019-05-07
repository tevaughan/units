/// @file       test/eigen-compat-test.cpp
/// @brief      Test-cases for compatibility with Eigen.
/// @copyright  2019 Thomas E. Vaughan; all rights reserved.
/// @license    BSD three-clause; see LICENSE.

#include "../vnix/units.hpp"
#include "catch.hpp"
#include <eigen3/Eigen/Geometry> // for AngleAxis, Matrix, etc.

using namespace vnix;


TEST_CASE("Matrix methods can be called.", "[dimval]") {
  using namespace Eigen;
  using namespace vnix::units;
  using namespace vnix::units::flt;
  AngleAxisf r(M_PI / 6, Vector3f(0, 0, 1));
  auto       f = newtons(Vector3f(1, 0, 0));
  REQUIRE((f[0] / N).to_number() == 1.0);
  REQUIRE((f[1] / N).to_number() == 0.0);
  REQUIRE((f[2] / N).to_number() == 0.0);
  auto f2 = r.toRotationMatrix() * f;
  REQUIRE((f2[0] / N).to_number() == Approx(cos(M_PI / 6)));
  REQUIRE((f2[1] / N).to_number() == Approx(sin(M_PI / 6)));
  REQUIRE((f2[2] / N).to_number() == 0.0);
  auto d = meters(Vector3f(0, 1, 0));
  REQUIRE((d[0] / m).to_number() == 0.0);
  REQUIRE((d[1] / m).to_number() == 1.0);
  REQUIRE((d[2] / m).to_number() == 0.0);
  auto v = d / (2.5_s);
  REQUIRE((v[0] / (m / s)).to_number() == Approx(0.0));
  REQUIRE((v[1] / (m / s)).to_number() == Approx(0.4));
  REQUIRE((v[2] / (m / s)).to_number() == Approx(0.0));
  flt::time t  = 3.0_s;
  auto      d2 = d + v * t;
  REQUIRE((d2[0] / m).to_number() == Approx(0.0));
  REQUIRE((d2[1] / m).to_number() == Approx(2.2));
  REQUIRE((d2[2] / m).to_number() == Approx(0.0));
  auto e1 = f2.dot(d2);
  REQUIRE((e1/J).to_number() == Approx(sin(M_PI / 6) * 2.2));
  auto e2 = f2.cross(d2);
  REQUIRE((e2[0]/J).to_number() == Approx(0.0));
  REQUIRE((e2[1]/J).to_number() == Approx(0.0));
  REQUIRE((e2[2]/J).to_number() == Approx(cos(M_PI / 6) * 2.2));
}

