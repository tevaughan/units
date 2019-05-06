# units: Header-only C++ Library for Physical Quantities

The vnix::units library defines types and constants that can be used to model
any quantity with physical dimension.

- no external dependencies needed to build against vnix::units
    - ruby required to build some header files before installation from
      git-clone
- compiler for C++-14 or later required
- interoperability with [Eigen](http://eigen.tuxfamily.org/) supported


## Continual Integration and Continually Generated Documentation

The [github-site for units](https://github.com/tevaughan/units) is connected to
[TravisCI](https://travis-ci.org), and thereby to
[CodeCov](https://codecov.io).

On each push to github, several actions are taken via TravisCI.

- Unit tests built and run, and the result is reported on
  [the units page at TravisCI](https://travis-ci.org/tevaughan/units).

- [kcov](https://github.com/SimonKagstrom/kcov) is used to run the unit-test
  executable, and the result is
  [published at CodeCov](https://codecov.io/gh/tevaughan/units).

- Doxygen is run on the tree of headers, the
  [result is published via github-pages](https://tevaughan.github.io/units/).

    - The markdown-based page that you are now reading is used as the main page
      for the doxygen-generated site.

    - TravisCI builds the documentation and pushes it to the gh-pages branch
      whenever there be an ordinary push to the master branch.

    - The instructions that I followed for setting this up are
      [here](https://github.com/EmaroLab/docs/wiki/Automatic-deployment-Doxygen-documentation).


## Overview of Library

In the present implementation, there are five fundamental dimensions:

- length,
- mass,
- time,
- charge, and
- temperature.

Internally,

- vnix::units::dim stores a six-bit rational exponent for each fundamental
  dimension, and
- vnix::units::dimval associates an instance of vnix::units::dim with numeric
  value.

However, the user need not even know about dim and dimval.
One may write a simple program, for example, as follows:
```cpp
#include <vnix/units.hpp>
using namespace std;
using namespace vnix::units::flt; // or vnix::units::dbl or vnix::units::ldbl
int main() {
  // Explicit namespace needed for 'time' in order to avoid collision with
  // function in C standard library.
  units::flt::time t  = 4 * ms;  // Multiplication of number by unit.
  length           d  = 3.0_km;  // Literal for unit.
  speed            v1 = d / t;   // Dimension stored in type of v1.
  dyndim           v2 = d / t;   // Dimension stored in instance of v2.
  cout << v1 << endl;
  return 0;
}
```
- The output looks like this: `750000 m s^-1`.
- See  test/dimval-test.cpp  for this and other examples.


## Some Details

- The library is extensible and reconfigurable via `units.yml`, which briefly
  defines the system of units.
    - A ruby script reads `units.yml` and generates
        - vnix/units.hpp and
        - vnix/units/dim-base-off.hpp.

- Because vnix::units::dimval is a literal type, an instance can be a [constant
  expression](https://en.cppreference.com/w/cpp/language/constant_expression).

- In the vnix::units library, many a standard unit, such as

    - vnix::units::flt::m for single-precision meter and
    - vnix::units::dbl::s for double-precision second,

  is defined as a constant expression.

- Many a dimension, such as

    - vnix::units::flt::length  (single precision),
    - vnix::units::dbl::mass  (double precision),
    - vnix::units::ldbl::force  (long-double precision),
    - vnix::units::flt::current,
    - etc.,

  is defined as a *type* that you can use.  (This type is a descendant of
  vnix::units::dimval).
  ```cpp
  using namespace vnix::units::flt; // For single-precision types.
  length l1 = 2.4 * m;              // Construct by multiplying unit.
  length l2 = 7.7_cm;               // Construct via user-defined literal.
  area   a  = l1 * l2;
  ```

- A variable of such a type knows its dimension without requiring any storage
  to keep track of the dimension.  The compiler keeps track and will find any
  bug related to a failure of dimensional analysis.
  ```cpp
  using namespace vnix::units::dbl; // For double-precision types.
  area a = 2.4 * m * 3.6;           // OOPS!  Compiler error: RHS not area!
  ```

- The `auto` keyword in modern C++ allows one not to have to specify the type
  when declaring a variable, and so a result of multiplication, division, and
  exponentiation can be stored even if there be no named type for the
  corresponding dimension.  (The type will then be a generic kind of
  vnix::units::statdim, itself a descendant of vnix::units::dimval.)
  ```cpp
  using namespace vnix::units::ldbl; // For long-double types.
  length foo = 3 * m;
  auto bar = sqrt(foo);              // No simple name for this type.
  ```

- vnix::units::flt::dyndim (or vnix::units::dbl::dyndim or
  vnix::units::ldbl::dyndim) can be used when the dimension cannot be
  determined at compile-time, such as when the dimension of a variable is to be
  read from a file at run-time.

- Using memory in the instance to store the dimension, vnix::units::flt::dyndim
  is unlike, say, vnix::units::flt::length, which does not use memory in the
  instance to store the dimension; yet vnix::units::flt::dyndim, too, is a
  descendant of vnix::units::dimval.

- The dimension stored in an instance of `dyndim` requires an extra four bytes
  of storage beyond the number bytes required to store the numeric value of the
  physical quantity.

- vnix::units::sqrt and vnix::units::pow are provided.


## Fetching, Building, and Installing

- `clang++-7` and `g++-8.3` are known to work.
    - Earlier versions might also work, but I have not tested them.
    - The default is clang++ because the Travis CI build-machine doesn't have a
      recent enough g++.

- To build the library from a clone of the source, be sure that ruby is
  installed.  A couple of the header files are generated from `units.yml`.

- See https://github.com/tevaughan/units

- To build the test and/or install:

  ```
  git clone https://github.com/tevaughan/units.git
  cd units      # Change the working directory into the new clone.
  vim Makefile  # Edit PREFIX for installation or CXX to change compiler.
  make test     # Build and run the tests.
  make doc      # Build the documentation via Doxygen.
  make install  # Install the headers to $(PREFIX)/include/vnix.
  ```


## License

Copyright 2019 Thomas E. Vaughan; all rights reserved.

Distributed according to the terms of the BSD three-clause license; see
LICENSE.
