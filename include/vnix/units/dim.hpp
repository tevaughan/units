/// @file       units/dim.hpp
/// @brief      Definition of vnix::units::dim.
/// @copyright  2019 Thomas E. Vaughan
/// @license    GPL, Version 3 or later.

#ifndef VNIX_UNITS_DIM_HPP
#define VNIX_UNITS_DIM_HPP

#include <algorithm>                    // for transform
#include <array>                        // for array
#include <vnix/units/impl/rational.hpp> // for rational

namespace vnix {
namespace units {


/// Offset of exponent for each base quantity.
enum base_off : uint8_t {
  TIM,      ///< Offset of exponent for time.
  LEN,      ///< Offset of exponent for length.
  MAS,      ///< Offset of exponent for mass.
  CHG,      ///< Offset of exponent for charge.
  TMP,      ///< Offset of exponent for temperature.
  NUM_BASES ///< Number of base quantities.
};


/// Composite dimension, which stores a rational exponent for each base
/// quantity.
class dim {
  using rat = impl::rat8_t;               ///< Short-hand.
  using arr = std::array<rat, NUM_BASES>; ///< Short-hand.
  arr e_;                                 ///< Storage for exponents.

public:
  /// Initialize exponents, one for each base quantity.
  /// @param d  Array of elements of rational type.
  dim(arr const &e) : e_(e) {}

  /// Initialize exponents, one for each base quantity.
  /// @tparam Rs  List of types, each of which should be rational type.
  /// @param  rs  List of instances of rational type.
  template <typename... Rs> constexpr dim(Rs... rs) : e_{rs...} {
    static_assert(sizeof...(rs) == NUM_BASES, "inappropriate length of list");
  }

  /// Rational exponent for base quantity at offset off.
  /// @param off  Offset.
  constexpr rat operator[](base_off off) const {
    if (off >= NUM_BASES) {
      throw "illegal offset";
    }
    return e_[off];
  }

  /// Combine each exponent in one input with the corresponding exponent in
  /// a second input to produce the corresponding exponent in the output.
  ///
  /// @tparam F    Type of binary function combining for combining the inputs.
  /// @param  i1   Reference to first input.
  /// @param  i2   Reference to second input.
  /// @param  out  Reference to output.
  /// @param  f    Binary function for combining the inputs.
  template <typename F>
  friend constexpr void combine(dim const &i1, dim const &i2, dim &out, F f) {
    arr::const_iterator iin1 = i1.e_.begin();
    arr::const_iterator iin2 = i2.e_.begin();
    arr::iterator       iout = out.e_.begin();
    while (iout != out.e_.end()) {
      *iout++ = f(*iin1++, *iin2++);
    }
  }

  /// Add corresponding exponents.
  /// This is called when two physical quantities are multiplied.
  /// Passing lamba in constexpr function requires C++17.
  /// @param a  Addends.
  /// @param s  Sums.
  constexpr void add(dim const &a, dim &s) const {
    combine(*this, a, s, [](rat const &r1, rat const &r2) { return r1 + r2; });
  }

  /// Negate exponents.
  ///
  /// This may be called when one physical quantity is divided by another or
  /// when a physical quantity is raised to a rational power.
  ///
  /// @param  Negatives.
  constexpr void negate(dim &n) {
    for (unsigned i = 0; i < NUM_BASES; ++i) {
      n.e_[i] = -e_[i];
    }
  }

  /// Subtract corresponding exponents.
  /// This is called when one physical quantity is divided by another.
  /// @param s  Subtrahends.
  /// @param d  Differences.
  constexpr void subtract(dim const &s, dim &d) const {
    for (unsigned i = 0; i < NUM_BASES; ++i) {
      d.e_[i] = e_[i] - s.e_[i];
    }
  }
};


} // namespace units
} // namespace vnix

#endif // ndef VNIX_DIM_HPP
