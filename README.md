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
  using namespace std;
  using namespace vnix::units;
  int main() {
    auto d = 3 * m;
    auto t = 4 * s;
    auto v = d / t;
    cout << speed << endl;
    return 0;
  }
  ```
  The output looks like this: `0.75 m s^-1`.

## Fetching, Building, and Installing

Install a C++ compiler that allows a constexpr function to throw an exception.

- By default, the build system requires that `clang++` be installed for
  building the tests.

- If you don't have `clang++` installed, then to build the tests you will have
  to override the setting of `CMAKE_CXX_COMPILER` that is used by
  `test/CMakeLists.txt`.

- `clang++-3.8` or later is known to work.  Earlier versions of `clang++` might
  also work, but I have not tested them.

- I have found that `g++-8.3` and lower do not work.

To build or to install, start with this:

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
