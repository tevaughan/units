/// @file       units/number.hpp
/// @brief      Definition of vnix::units::number.
/// @copyright  2019  Thomas E. Vaughan
/// @license    GPL Version 3 or later.

#ifndef VNIX_UNITS_NUMBER_HPP
#define VNIX_UNITS_NUMBER_HPP

namespace vnix {
namespace units {


/// Protected storage for numeric value associated with a physical unit.
///
/// The number is protected because it is here abstracted away from the unit,
/// which it needs in order to make any sense.  The number is combined with the
/// unit in a descendant class.
///
/// @tparam T  Type of number, typically `float` or `double`.
template <typename T> class number {
protected:
  T v_; ///< Numeric value that multiplies units.

  /// Initialize numeric value and exponents of units.
  /// @param v  Numeric value that multiplies units.
  constexpr number(T v) : v_(v) {}
};


} // namespace units
} // namespace vnix

#endif // ndef VNIX_UNITS_NUMBER_HPP
