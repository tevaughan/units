# `units`: Header-only C++ Library for Physical Quantities

Requiring C++-14 or later, the `units` library defines a literal type `dimval`,
which models a quantity that is dimensioned according to fundamental physical
units:

- meter (for length)
- kilogram (for mass)
- second (for time)
- Coulomb (for charge)
- Kelvin (for temperature)

The user need not use class `dimval` directly.

- Because `dimval` is a literal type, an instance can be a constant expresion
  (whose value is known at compile-time).

- In the `units` library, many a standard unit (such as `m` for meter and `s`
  for second) is defined as a constant expression.

- So, for example, one may write a simple program as follows:
  ```c++
  #include <vnix/units.hpp>
  using namespace vnix::units;
  int main() {
    auto length = 3 * m;
    auto time   = 4 * s;
    auto speed  = length / time;
    std::cout << speed << std::endl;
    return 0;
  }
  ```
  The output looks like this: `0.75 m s^-1`.

## Fetching, Building, and Installing

To build or to install start with this:

```
git clone https://github.com/tevaughan/units.git
cd units
mkdir build
cd build
cmake [-DCMAKE_INSTAll_PREFIX=<custom prefix>] ..
```

If you just want to build the tests:

```
make
```

If you want to install the headers:

```
make install
```

## License

GPL3

Copyright 2019  Thomas E. Vaughan

See [LICENSE](LICENSE).
