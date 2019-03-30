
#include "units.hpp"

using namespace vnix;
int main() {
  auto constexpr a = 3 * J;
  auto constexpr b = 2 * C;
  auto constexpr c = 4.3 * μm;
  std::cout << a / b * c << " " << std::endl;
  try {
    std::cout << double(a / b) << std::endl;
  } catch (char const *e) {
    std::cerr << e << std::endl;
    return 1;
  }
  return 0;
}
