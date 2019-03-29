# C++ Library for Physical Quantities

This version does not use templates but rather defines a literal type `dimval`.

Yet the user need not use `dimval` directly.  Many a standard unit is defined
as a constant expression.  So, for example,

```c++
#include <iostream>
#include "units.hpp"
using namespace vnix;
int main() {
  auto length = 3*m;
  auto time   = 4*s;
  auto speed  = length / time;
  std::cout << speed << std::endl;
  return 0;
}
```

The output looks like this: `0.75 m s^-1`.
