
#include <vnix/rational.hpp>
#include <vnix/unitsf.hpp>

using namespace vnix::units;
using namespace vnix::rat;


int main() {
  try {
    auto constexpr a = 3 * J;
    auto constexpr b = 2 * C;
    auto constexpr c = 4.3 * micron;
    std::cout << a / b * c << std::endl;
    std::cout << sqrt(2 * s) << std::endl;
    std::cout << pow<-1, 2>(2 * s) << std::endl;
    vnix::rat8_t r(-6, 4);
    std::cout << r + vnix::rat8_t(1, 8) << std::endl;
    std::cout << a / J << std::endl;
    dyndimd d = a / b;
    std::cout << d << std::endl;
    std::cout << statdimf<len_dim>(d) << std::endl;
  } catch (char const *e) {
    std::cerr << e << std::endl;
    return 1;
  }
  return 0;
}
