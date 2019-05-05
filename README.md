# units: Header-only C++ Library for Physical Quantities

The vnix::units library defines a [literal
type](https://en.cppreference.com/w/cpp/named_req/LiteralType)
vnix::units::dimval that models a quantity with physical dimension.

- requires C++-14 or later
- no external dependencies
- [documentation](https://tevaughan.github.io/units/)
- [coverage-report for unit-tests](https://tevaughan.github.io/units/kcov/index.html)

In the present implementation, there are five fundamental dimensions:

- length,
- mass,
- time,
- charge, and
- temperature.

The user need not use vnix::units::dimval directly.

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

- The dimension of a variable is specified by a set of five rational exponents,
  one for each of the five fundamental dimensions.

- The `auto` keyword in modern C++ allows one not to have to specify the type
  when declaring a variable, and so a result of multiplication, division, and
  exponentiation is legal even if there be no named type for the corresponding
  dimension.  (The type will then be a generic kind of vnix::units::statdim,
  itself a descendant of vnix::units::dimval.)
  ```cpp
  using namespace vnix::units::ldbl; // For long-double types.
  length foo = 3 * m;
  auto bar = sqrt(foo);              // No simple name for this type.
  ```

- Finally, the type vnix::units::flt::dyndim (or vnix::units::dbl::dyndim or
  vnix::units::ldbl::dyndim) can be used when the dimension cannot be
  determined at compile-time, such as when the dimension of a variable is to be
  read from a file at run-time.

- Using memory in the instance to store the dimension, vnix::units::flt::dyndim
  is unlike, say, vnix::units::flt::length, which does not use memory in the
  instance to store the dimension; yet vnix::units::flt::dyndim, too, is a
  descendant of vnix::units::dimval.

- In the case of vnix::units::flt::dyndim, the instance consumes an extra four
  bytes for the dimension (in addition to the four bytes used by the
  single-precision floating-point number that is stored when using, say,
  vnix::units::flt::length).

- One may write a simple program, for example, as follows:
  ```cpp
  #include <vnix/units.hpp>
  using namespace std;
  using namespace vnix::units::flt; // or vnix::units::dbl or vnix::units::ldbl
  int main() {
    // Explicit namespace needed for 'time' in order to avoid collision with
    // function in C standard library.
    length      d = 3.0_km;  // User-defined literal.
    units::time t = 4 * ms;  // Multiplication of number by unit.
    dyndim      v = d / t;
    cout << v << endl;
    return 0;
  }
  ```
    - The output looks like this: `750000 m s^-1`.
    - See  test/dimval-test.cpp  for this and other examples.

- vnix::units::sqrt and vnix::units::pow are provided.


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
  make test     # Build and run the tests.
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
