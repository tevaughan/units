
#include <vnix/units.hpp>
#include <vnix/units/impl/rational.hpp>

using namespace vnix::units;
int main() {
  auto constexpr a = 3 * J;
  auto constexpr b = 2 * C;
  auto constexpr c = 4.3 * μm;
  std::cout << a / b * c << " " << std::endl;
  try {
    impl::rat8_t r(-6,4);
    std::cout << r + impl::rat8_t(1,8) << std::endl;
    //std::cout << double(a / b) << std::endl;
  } catch (char const *e) {
    std::cerr << e << std::endl;
    return 1;
  }
  return 0;
}
