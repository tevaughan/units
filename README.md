# units: Header-only C++ Library for Physical Quantities

Requiring C++-14 or later, the vnix::units library defines a [literal
type](https://en.cppreference.com/w/cpp/named_req/LiteralType)
vnix::units::dimval that models a quantity with physical dimension.

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
    - vnix::units::m for meter and
    - vnix::units::s for second,

  is defined as a constant expression.

- Many a dimension, such as
    - vnix::units::length,
    - vnix::units::mass,
    - vnix::units::force,
    - vnix::units::current,
    - etc.,

  is defined as a *type* that you can use.  (This type is a descendant of
  vnix::units::dimval).
  ```cpp
  using namespace vnix::units;
  length l1 = 2.4 * m;
  length l2 = 7.7 * cm;
  area   a  = l1 * l2;
  ```

- A variable of such a type knows its dimension without requiring any storage
  to keep track of the dimension.  The compiler keeps track and will find any
  bug related to a failure of dimensional analysis.
  ```cpp
  using namespace vnix::units;
  area a = 2.4 * m * 3.6; // OOPS!  Compiler generates error.
  ```

- The dimension of a variable is specified by a set of five rational exponents,
  one for each of the five fundamental dimensions.

- The `auto` keyword in modern C++ allows one not to have to specify the type
  when declaring a variable, and so a result of multiplication, division, and
  exponentiation is legal even if there be no named type for the corresponding
  dimension.  (The type will then be a generic kind of vnix::units::statdim,
  itself a descendant of vnix::units::dimval.)
  ```cpp
  using namespace vnix::units;
  length foo = 3 * m;
  auto bar = sqrt(foo);  // No simple name for this type.
  ```

- Finally, the type vnix::units::dyndim can be used when the dimension cannot
  be determined at compile-time, such as when the dimension of a variable is to
  be read from a file at run-time.

- Using memory in the instance to store the dimension, vnix::units:dyndim is
  unlike, say, vnix::units::length, which does not use memory in the instance
  to store the dimension; yet vnix::units::dyndim, too, is a descendant of
  vnix::units::dimval.

- In the case of vnix::units::dyndim, the instance consumes an extra eight
  bytes for the dimension (in addition to the eight bytes used by the
  double-precision number that is stored when using the double-precision
  version of, say, vnix::units::length).

- One may write a simple program, for example, as follows:
  ```cpp
  #include <vnix/unitsd.hpp>
  // Use <vnix/unitsf.hpp> for single-precision units.
  using namespace std;
  using namespace vnix::units;
  int main() {
    // Explicit namespace needed for 'time' in order to avoid collision with
    // function in C standard library.
    length      d = 3 * km;
    units::time t = 4 * ms;
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
