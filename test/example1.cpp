
#include <eigen3/Eigen/Core>
#include <iostream>
#include <vnix/units.hpp>



int main() {
  auto m1 = Eigen::Matrix3f::Identity();
  auto v1 = Eigen::Vector3f(1, 2, 3);
  auto f  = vnix::units::newtons(m1);
  auto dx = vnix::units::meters(v1);
  std::cout << f * dx << std::endl;
  return 0;
}

