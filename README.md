# units: Header-only C++ Library for Physical Quantities

Requiring C++-14 or later, the `units` library defines a literal type
vnix::units::dimval, which models a quantity that is dimensioned according to
fundamental physical units:

- meter (for length)
- kilogram (for mass)
- second (for time)
- Coulomb (for charge)
- Kelvin (for temperature)

The user need not use class `dimval` directly.

- Because `dimval` is a literal type, an instance can be a constant expression
  (whose value is known at compile-time).

- In the vnix::units library, many a standard unit (such as vnix::units::m for
  meter and vnix::units::s for second) is defined as a constant expression.

- So, for example, one may write a simple program as follows:
  ```
  #include <vnix/unitsd.hpp>
  // Use <vnix/unitsf.hpp> for single-precision units.
  using namespace std;
  using namespace vnix::units;
  int main() {
    length      d = 3 * km;
    units::time t = 4 * ms;
    auto        v = d / t;
    cout << v << endl;
    return 0;
  }
  ```
  The output looks like this: `7.5E+05 m s^-1`.

- sqrt() and pow() are supported.
    - vnix::units::sqrt
    - vnix::units::pow


## Fetching, Building, and Installing

- `clang++-7` and `g++-8.3` are known to work.
    - Earlier versions might also work, but I have not tested them.
    - The default is clang++ because the Travis CI build-machine doesn't have a
      recent enough g++.

- See https://github.com/tevaughan/units

- To build the test and/or install:

  ```
  git clone https://github.com/tevaughan/units.git
  cd units      # Change the working directory into the new clone.
  vim Makefile  # Edit PREFIX for installation or CXX to change compiler.
  make          # Build and run the tests.
  make doc      # Build the documentation via Doxygen.
  make install  # Install the headers to $(PREFIX)/include/vnix.
  ```


## Documentation

https://tevaughan.github.io/units/

(The documentation site is generated via a gh-pages branch to which Travis CI
can push whenever there be an ordinary push to the master branch.  The
instructions on how to do this are
[here](https://github.com/EmaroLab/docs/wiki/Automatic-deployment-Doxygen-documentation).)


## License

Copyright 2019  Thomas E. Vaughan

Distributed according to the terms of the GNU GPL, Version 3 or later.
