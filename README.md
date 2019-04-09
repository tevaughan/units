# `units`: Header-only C++ Library for Physical Quantities

Requiring C++-17 or later, the `units` library defines a literal type `dimval`,
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
    cout << v << endl;
    return 0;
  }
  ```
  The output looks like this: `0.75 m s^-1`.

## Fetching, Building, and Installing

- At present, `clang++` must be installed for building against the library.

- `clang++-7` or later is known to work.  Earlier versions of `clang++` might
  also work, but I have not tested them.

- I have found that `g++-8.3` and lower do not work.

    - g++-8.3 does not allow a constexpr function to throw.

    - g++-8.3 has trouble using with computing a template-value parameter when
      the expression for the parameter evaluates to a constexpr class that has
      a conversion operator to uint64_t.

To build the test and/or install:

```
git clone https://github.com/tevaughan/units.git
cd units         # Change the working directory into the new clone.
make             # Builds the test program.
test/units-test  # Run the test program.
vim Makefile     # Edit PREFIX for installation.
make install     # Install the headers to $(PREFIX)/include/vnix.
```

## License

GPL3

Copyright 2019  Thomas E. Vaughan

See [LICENSE](LICENSE).
