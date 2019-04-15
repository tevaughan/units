/// @file       rational.hpp
/// @brief      Definition of vnix::rational.
/// @copyright  2019  Thomas E. Vaughan
/// @license    GPL Version 3 or later.

#ifndef VNIX_RATIONAL_HPP
#define VNIX_RATIONAL_HPP

#include <iostream>
#include <vnix/rat/encoding.hpp>

namespace vnix {


/// Model of a fixed-precision rational number.
///
/// Numerator and denominator are encoded in same unsigned integer word.  They
/// form a partition of the bits in the word.
///
/// @tparam U  Type of unsigned integer word.
template <typename U> class rational : public rat::encoding<U> {
  struct dummy_arg {};

  /// Construct from encoding.
  /// @param u  Encoding of rational number.
  constexpr rational(U u, dummy_arg) : rat::encoding<U>(u) {}

public:
  using typename rat::rational_base<U>::S;
  using rat::encoding<U>::n;
  using rat::encoding<U>::d;

  /// Initialize representation.
  /// @param n  Numerator.
  /// @param d  Denominator.
  constexpr rational(int64_t n = 0, int64_t d = 1)
      : rat::encoding<U>(rat::normalized_pair<U>(n, d)) {}

  /// Automatically convert to (signed) integer.
  constexpr operator S() const {
    if (d() != 1) {
      throw "attempted conversion to integer from fraction";
    }
    return n();
  }

  /// Automatically convert to boolean.
  constexpr operator bool() const { return n() != 0; }

  /// Compare for equality with another rational.
  constexpr bool operator==(rational r) const {
    return n() == r.n() && d() == r.d();
  }

  /// Compare for inequality with another rational.
  constexpr bool operator!=(rational r) const {
    return n() != r.n() || d() != r.d();
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
    U const g   = gcd(d1, d2);
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
    U const gb = gcd((n2 < 0 ? -n2 : n2), d1);
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
    if (d() != 1) {
      s << "[";
    }
    s << int64_t(n());
    if (d() != 1) {
      s << '/' << uint64_t(d()) << "]";
    }
    return s;
  }

  /// Encoding from rational number.
  constexpr static U encode(rational r) { return r.c_; }

  /// Rational number from encoding.
  constexpr static rational decode(U u) { return {u, dummy_arg()}; }
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


} // namespace vnix

#endif // ndef VNIX_RATIONAL_HPP