/// @file       units/impl/rational.hpp
/// @brief      Definition of vnix::units::impl::rational.
/// @copyright  2019 Thomas E. Vaughan
/// @license    GPL3.

#ifndef VNIX_UNITS_IMPL_RATIONAL_HPP
#define VNIX_UNITS_IMPL_RATIONAL_HPP

#include <iostream>
#include <vnix/units/impl/rat/encoding.hpp>

namespace vnix {
namespace units {
namespace impl {


/// Model of a fixed-precision rational number.
///
/// Numerator and denominator are encoded in same unsigned integer word.  They
/// form a partition of the bits in the word.
///
/// @tparam U  Type of unsigned integer word.
template <typename U> struct rational : public rat::encoding<U> {
  /// Initialize representation.
  /// @param n  Numerator.
  /// @param d  Denominator.
  constexpr rational(int64_t n = 0, int64_t d = 1)
      : rat::encoding<U>(rat::normalized_pair<U>(n, d)) {}

  using typename rat::rational_base<U>::S;
  using rat::encoding<U>::n;
  using rat::encoding<U>::d;

  /// Automatically convert to (signed) integer.
  constexpr operator S() const {
    if (d() != 1) {
      throw "attempted conversion to integer from fraction";
    }
    return n();
  }

  /// Copy rational number.
  /// @param r  Number to copy.
  /// @return   Copy of number.
  friend constexpr rational operator+(rational r) { return r; }

  /// Negate a rational number.
  /// @param r  Number to negate.
  /// @return   Negative of number.
  friend constexpr rational operator-(rational r) { return {-r.n(), r.d()}; }

  /// Sum of two rational numbers.
  /// @param r1  Addend.
  /// @param r2  Adder.
  /// @return    Sum.
  friend constexpr rational operator+(rational r1, rational r2) {
    U const d1  = r1.d();
    U const d2  = r2.d();
    U const g   = rat::gcd(d1, d2);
    U const d1g = d1 / g;
    U const d2g = d2 / g;
    return {r1.n() * d2g + r2.n() * d1g, d1g * d2};
  }

  /// Difference between two rational numbers.
  /// @param r1  Minuend.
  /// @param r2  Subtrahend.
  /// @return    Difference.
  friend constexpr rational operator-(rational r1, rational r2) {
    return -r2 + r1;
  }

  /// Modify this rational number by adding other rational number.
  /// @param r  Other rational number.
  /// @return   Reference to this rational number (after addition).
  constexpr rational &operator+=(rational r) { return *this = *this + r; }

  /// Modify this rational number by subtracting other rational number.
  /// @param r  Other rational number.
  /// @return   Reference to this rational number (after subtraction).
  constexpr rational &operator-=(rational r) { return *this = *this - r; }

  /// Reciprocal of this rational number.
  constexpr rational reciprocal() const {
    if (n() == 0) {
      throw "attempt to take reciprocal of zero";
    }
    if (n() < 0) {
      return {-d(), -n()};
    }
    return {d(), n()};
  }

  /// Product of two rational numbers.
  /// @param r1  Multiplicand.
  /// @param r2  Multiplier.
  /// @return    Product.
  friend constexpr rational operator*(rational r1, rational r2) {
    S const n1 = r1.n();
    U const d1 = r1.d();
    S const n2 = r2.n();
    U const d2 = r2.d();
    U const ga = gcd((n1 < 0 ? -n1 : n1), d2);
    U const gb = gdc((n2 < 0 ? -n2 : n2), d1);
    return {n1 / ga * n2 / gb, d1 / gb * d2 / ga};
  }

  /// Quotient of two rational numbers.
  /// @param r1  Dividend.
  /// @param r2  Divisor.
  /// @return    Quotient.
  friend constexpr rational operator/(rational r1, rational r2) {
    return r1 * r2.reciprocal();
  }

  /// Modify this rational number by multiplying by other rational number.
  /// @param r  Other rational number.
  /// @return   Reference to this rational number (after multiplication).
  constexpr rational &operator*=(rational r) { return *this = (*this) * r; }

  /// Modify this rational number by dividing by other rational number.
  /// @param r  Other rational number.
  /// @return   Reference to this rational number (after division).
  constexpr rational &operator/=(rational r) { return *this = (*this) / r; }

  /// Print this rational number to output-stream.
  /// @param s  Reference to output-stream.
  /// @return   Reference to modified output-stream.
  std::ostream &print(std::ostream &s) const {
    return s << int64_t(n()) << '/' << uint64_t(d());
  }
};


/// Print rational number to output-stream.
/// @tparam U  Type of word in which numerator and denominator are encoded.
/// @param  s  Reference to output-stream.
/// @param  r  Rational number.
/// @return    Reference to modified output-stream.
template <typename U>
std::ostream &operator<<(std::ostream &s, rational<U> r) {
  return r.print(s);
}


using rat8_t  = rational<uint8_t>;  ///< Short-hand.
using rat16_t = rational<uint16_t>; ///< Short-hand.
using rat32_t = rational<uint32_t>; ///< Short-hand.
using rat64_t = rational<uint64_t>; ///< Short-hand.


} // namespace impl
} // namespace units
} // namespace vnix

#endif // ndef VNIX_UNITS_IMPL_RATIONAL_HPP
