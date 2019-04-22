/// @file       vnix/rat/rational.hpp
/// @brief      Definition of vnix::rat::rational.
/// @copyright  2019  Thomas E. Vaughan
/// @license    GPL Version 3 or later.

#ifndef VNIX_RAT_RATIONAL_HPP
#define VNIX_RAT_RATIONAL_HPP

#include <iostream>
#include <vnix/rat/common-denom.hpp>
#include <vnix/rat/encoding.hpp>

namespace vnix {

/// Classes and functions supporting a model of a fixed-precision rational
/// number.
namespace rat {


/// Model of a fixed-precision rational number.
///
/// Numerator and denominator are encoded in same unsigned integer word.  They
/// form a partition of the bits in the word.
///
/// @tparam NB  Number of bits for numerator.
/// @tparam DB  Number of bits for denominator.
template <unsigned NB, unsigned DB> class rational : public encoding<NB, DB> {
  struct dummy_arg {};
  using P = encoding<NB, DB>;
  using typename P::type;

  /// Construct from encoding.
  /// @param u  Encoding of rational number.
  constexpr rational(type u, dummy_arg) : P(u) {}

public:
  using P::d;
  using P::n;
  using typename P::SF;
  using typename P::UF;

  /// Initialize from numerator and denominator.
  /// @param n  Numerator   (zero  by default).
  /// @param d  Denominator (unity by default).
  constexpr rational(SF n = 0, SF d = 1) : P(normalized_pair<NB, DB>(n, d)) {}

  /// Initialize from other rational.
  /// @tparam ONB  Number of numerator-bits in other type of rational.
  /// @tparam ODB  Number of denominator-bits in other type of rational.
  /// @param  r    Copy of other rational.
  template <unsigned ONB, unsigned ODB>
  constexpr rational(rational<ONB, ODB> r)
      : P(normalized_pair<NB, DB>(r.n(), r.d())) {}

  /// Convert to (signed) integer.
  constexpr SF to_int() const {
    if (d() != 1) { throw "attempted conversion to integer from fraction"; }
    return n();
  }

  /// Automatically convert to boolean.
  constexpr bool to_bool() const { return n() != 0; }

  /// Convert to float.
  constexpr double to_float() const { return n() * 1.0f / d(); }

  /// Convert to double.
  constexpr double to_double() const { return n() * 1.0 / d(); }

  /// Modify this rational number by adding other rational number.
  /// @param r  Other rational number.
  /// @return   Reference to this rational number (after addition).
  constexpr rational &operator+=(rational r);

  /// Modify this rational number by subtracting other rational number.
  /// @param r  Other rational number.
  /// @return   Reference to this rational number (after subtraction).
  constexpr rational &operator-=(rational r);

  /// Reciprocal of this rational number.
  constexpr rational reciprocal() const {
    if (n() == 0) { throw "attempt to take reciprocal of zero"; }
    if (n() < 0) { return rational(-d(), -n()); }
    return rational(d(), n());
  }

  /// Modify this rational number by multiplying by other rational number.
  /// @param r  Other rational number.
  /// @return   Reference to this rational number (after multiplication).
  constexpr rational &operator*=(rational r);

  /// Modify this rational number by dividing by other rational number.
  /// @param r  Other rational number.
  /// @return   Reference to this rational number (after division).
  constexpr rational &operator/=(rational r);

  /// Encoding from rational number.
  constexpr static UF encode(rational r) { return r.c_; }

  /// Rational number from encoding.
  constexpr static rational decode(UF u) { return {u, dummy_arg()}; }
};


/// Compare rationals for equality.
/// Promote both to uint64_t-storage for comparison.
/// @param r1  Left -hand operand.
/// @param r2  Right-hand operand.
constexpr bool operator==(rational<33, 31> r1, rational<33, 31> r2) {
  return r1.n() == r2.n() && r1.d() == r2.d();
}


/// Compare rationals for inequality.
/// Promote both to uint64_t-storage for comparison.
/// @param r1  Left -hand operand.
/// @param r2  Right-hand operand.
constexpr bool operator!=(rational<33, 31> r1, rational<33, 31> r2) {
  return !(r1 == r2);
}


/// Compare for less-than-or-equal ordering with another rational.
/// Promote both to uint64_t-storage for comparison.
/// @param r1  Left -hand operand.
/// @param r2  Right-hand operand.
constexpr bool operator<=(rational<33, 31> r1, rational<33, 31> r2) {
  common_denom const c(r1, r2);
  return c.n1 <= c.n2;
}


/// Compare for less-than ordering with another rational.
/// Promote both to uint64_t-storage for comparison.
/// @param r1  Left -hand operand.
/// @param r2  Right-hand operand.
constexpr bool operator<(rational<33, 31> r1, rational<33, 31> r2) {
  rat::common_denom const c(r1, r2);
  return c.n1 < c.n2;
}


/// Compare for greater-than-or-equal ordering with another rational.
/// Promote both to uint64_t-storage for comparison.
/// @param r1  Left -hand operand.
/// @param r2  Right-hand operand.
constexpr bool operator>=(rational<33, 31> r1, rational<33, 31> r2) {
  common_denom const c(r1, r2);
  return c.n1 >= c.n2;
}


/// Compare for greater-than ordering with another rational.
/// Promote both to uint64_t-storage for comparison.
/// @param r1  Left -hand operand.
/// @param r2  Right-hand operand.
constexpr bool operator>(rational<33, 31> r1, rational<33, 31> r2) {
  rat::common_denom const c(r1, r2);
  return c.n1 > c.n2;
}


/// Copy rational number by way of unary operator+.
/// @tparam NB  Number of bits for numerator.
/// @tparam DB  Number of bits for denominator.
/// @param  r         Number to copy.
/// @return           Copy of number.
template <unsigned NB, unsigned DB>
constexpr rational<NB, DB> operator+(rational<NB, DB> r) {
  return r;
}


/// Negate rational number.
/// @tparam NB  Number of bits for numerator.
/// @tparam DB  Number of bits for denominator.
/// @param  r         Number to negate.
/// @return           Negative of number.
template <unsigned NB, unsigned DB>
constexpr rational<NB, DB> operator-(rational<NB, DB> r) {
  return rational<NB, DB>(-r.n(), r.d());
}


/// Sum of two rational numbers.
/// @tparam U   Type of unsigned word in which rational is encoded.
/// @param  r1  Addend.
/// @param  r2  Adder.
/// @return     Sum.
template <unsigned NB, unsigned DB>
constexpr rational<NB, DB> operator+(rational<NB, DB> r1,
                                     rational<NB, DB> r2) {
  rat::common_denom const c(r1, r2);
  return rational<NB, DB>(c.n1 + c.n2, c.lcd);
}


// Modify rational number by adding other rational number.
template <unsigned NB, unsigned DB>
constexpr rational<NB, DB> &rational<NB, DB>::operator+=(rational<NB, DB> r) {
  return *this = *this + r;
}


/// Difference between two rational numbers.
/// @tparam U   Type of unsigned word in which rational is encoded.
/// @param  r1  Minuend.
/// @param  r2  Subtrahend.
/// @return     Difference.
template <unsigned NB, unsigned DB>
constexpr rational<NB, DB> operator-(rational<NB, DB> r1,
                                     rational<NB, DB> r2) {
  return -r2 + r1;
}


// Modify this rational number by subtracting other rational number.
template <unsigned NB, unsigned DB>
constexpr rational<NB, DB> &rational<NB, DB>::operator-=(rational<NB, DB> r) {
  return *this = *this - r;
}


/// Product of two rational numbers.
/// @tparam U   Type of unsigned word in which rational is encoded.
/// @param  r1  Multiplicand.
/// @param  r2  Multiplier.
/// @return     Product.
template <unsigned NB, unsigned DB>
constexpr rational<NB, DB> operator*(rational<NB, DB> r1,
                                     rational<NB, DB> r2) {
  typename rational<NB, DB>::SF const n1 = r1.n();
  typename rational<NB, DB>::SF const n2 = r2.n();
  typename rational<NB, DB>::SF const d1 = r1.d();
  typename rational<NB, DB>::SF const d2 = r2.d();
  typename rational<NB, DB>::SF const ga = gcd((n1 < 0 ? -n1 : n1), d2);
  typename rational<NB, DB>::SF const gb = gcd((n2 < 0 ? -n2 : n2), d1);
  return {typename rational<NB, DB>::SF(n1 / ga * n2 / gb),
          typename rational<NB, DB>::SF(d1 / gb * d2 / ga)};
}


/// Quotient of two rational numbers.
/// @tparam U   Type of unsigned word in which rational is encoded.
/// @param  r1  Dividend.
/// @param  r2  Divisor.
/// @return     Quotient.
template <unsigned NB, unsigned DB>
constexpr rational<NB, DB> operator/(rational<NB, DB> r1,
                                     rational<NB, DB> r2) {
  return r1 * r2.reciprocal();
}


// Modify this rational number by multiplying by other rational number.
template <unsigned NB, unsigned DB>
constexpr rational<NB, DB> &rational<NB, DB>::operator*=(rational<NB, DB> r) {
  return *this = (*this) * r;
}


// Modify this rational number by dividing by other rational number.
template <unsigned NB, unsigned DB>
constexpr rational<NB, DB> &rational<NB, DB>::operator/=(rational<NB, DB> r) {
  return *this = (*this) / r;
}


/// Print rational number to output-stream.
/// @tparam U  Type of unsigned word in which rational is encoded.
/// @param  s  Reference to output-stream.
/// @param  r  Rational number.
/// @return    Reference to modified output-stream.
template <unsigned NB, unsigned DB>
std::ostream &operator<<(std::ostream &s, rational<NB, DB> r) {
  s << int_fast64_t(r.n());
  if (r.d() != 1) { s << '/' << uint_fast64_t(r.d()); }
  return s;
}


} // namespace rat
} // namespace vnix

#endif // ndef VNIX_RAT_RATIONAL_HPP
