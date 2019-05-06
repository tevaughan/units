/// @file       vnix/rat/rational.hpp
/// @brief      Definition of vnix::rat::rational and operators for it.
/// @copyright  2019 Thomas E. Vaughan; all rights reserved.
/// @license    BSD Three-Clause; see LICENSE.

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

public:
  using typename P::stype;
  using typename P::utype;

private:
  /// Construct from encoding.
  /// @param u  Encoding of rational number.
  constexpr rational(utype u, dummy_arg) : P(u) {}

public:
  using P::d;
  using P::n;

  /// Initialize from numerator and denominator.
  /// @param n  Numerator   (zero  by default).
  /// @param d  Denominator (unity by default).
  constexpr rational(stype n = 0, stype d = 1)
      : P(normalized_pair<NB, DB>(n, d)) {}

  /// Initialize from other rational.
  /// @tparam ONB  Number of numerator-bits in other type of rational.
  /// @tparam ODB  Number of denominator-bits in other type of rational.
  /// @param  r    Copy of other rational.
  template <unsigned ONB, unsigned ODB>
  constexpr rational(rational<ONB, ODB> r)
      : P(normalized_pair<NB, DB>(r.n(), r.d())) {}

  /// Convert to (signed) integer.
  constexpr stype to_int() const {
    if (d() != 1) { throw "attempted conversion to integer from fraction"; }
    return n();
  }

  /// Automatically convert to boolean.
  constexpr bool to_bool() const { return n() != 0; }

  /// Convert to float.
  constexpr float to_float() const { return n() * 1.0f / d(); }

  /// Convert to double.
  constexpr double to_double() const { return n() * 1.0 / d(); }

  /// Modify this rational number by adding other rational number.
  /// @tparam ONB  Number of numerator  -bits in other rational.
  /// @tparam ODB  Number of denominator-bits in other rational.
  /// @param r     Other rational number.
  /// @return      Reference to this rational number (after addition).
  template <unsigned ONB, unsigned ODB>
  constexpr rational &operator+=(rational<ONB, ODB> r);

  /// Modify this rational number by subtracting other rational number.
  /// @tparam ONB  Number of numerator  -bits in other rational.
  /// @tparam ODB  Number of denominator-bits in other rational.
  /// @param r     Other rational number.
  /// @return      Reference to this rational number (after subtraction).
  template <unsigned ONB, unsigned ODB>
  constexpr rational &operator-=(rational<ONB, ODB> r);

  /// Reciprocal of this rational number.
  constexpr rational reciprocal() const {
    if (n() == 0) { throw "attempt to take reciprocal of zero"; }
    if (n() < 0) { return rational(-d(), -n()); }
    return rational(d(), n());
  }

  /// Modify this rational number by multiplying by other rational number.
  /// @tparam ONB  Number of numerator  -bits in other rational.
  /// @tparam ODB  Number of denominator-bits in other rational.
  /// @param r     Other rational number.
  /// @return      Reference to this rational number (after multiplication).
  template <unsigned ONB, unsigned ODB>
  constexpr rational &operator*=(rational<ONB, ODB> r);

  /// Modify this rational number by dividing by other rational number.
  /// @tparam ONB  Number of numerator  -bits in other rational.
  /// @tparam ODB  Number of denominator-bits in other rational.
  /// @param r     Other rational number.
  /// @return      Reference to this rational number (after division).
  template <unsigned ONB, unsigned ODB>
  constexpr rational &operator/=(rational<ONB, ODB> r);

  /// Encoding from rational number.
  constexpr static utype encode(rational r) { return r.c_; }

  /// Rational number from encoding.
  constexpr static rational decode(utype u) { return {u, dummy_arg()}; }
};


/// Compare rationals for equality.
/// @tparam NB1  Number of numerator  -bits in left -hand rational.
/// @tparam DB1  Number of denominator-bits in left -hand rational.
/// @tparam NB2  Number of numerator  -bits in right-hand rational.
/// @tparam DB2  Number of denominator-bits in right-hand rational.
/// @param  r1   Left -hand operand.
/// @param  r2   Right-hand operand.
template <unsigned NB1, unsigned DB1, unsigned NB2, unsigned DB2>
constexpr bool operator==(rational<NB1, DB1> r1, rational<NB2, DB2> r2) {
  return r1.n() == r2.n() && r1.d() == r2.d();
}


/// Compare rationals for inequality.
/// @tparam NB1  Number of numerator  -bits in left -hand rational.
/// @tparam DB1  Number of denominator-bits in left -hand rational.
/// @tparam NB2  Number of numerator  -bits in right-hand rational.
/// @tparam DB2  Number of denominator-bits in right-hand rational.
/// @param  r1   Left -hand operand.
/// @param  r2   Right-hand operand.
template <unsigned NB1, unsigned DB1, unsigned NB2, unsigned DB2>
constexpr bool operator!=(rational<NB1, DB1> r1, rational<NB2, DB2> r2) {
  return !(r1 == r2);
}


/// Compare for less-than-or-equal ordering with another rational.
/// @tparam NB1  Number of numerator  -bits in left -hand rational.
/// @tparam DB1  Number of denominator-bits in left -hand rational.
/// @tparam NB2  Number of numerator  -bits in right-hand rational.
/// @tparam DB2  Number of denominator-bits in right-hand rational.
/// @param  r1   Left -hand operand.
/// @param  r2   Right-hand operand.
template <unsigned NB1, unsigned DB1, unsigned NB2, unsigned DB2>
constexpr bool operator<=(rational<NB1, DB1> r1, rational<NB2, DB2> r2) {
  auto const c = common_denom(r1, r2);
  return c.n1 <= c.n2;
}


/// Compare for less-than ordering with another rational.
/// @tparam NB1  Number of numerator  -bits in left -hand rational.
/// @tparam DB1  Number of denominator-bits in left -hand rational.
/// @tparam NB2  Number of numerator  -bits in right-hand rational.
/// @tparam DB2  Number of denominator-bits in right-hand rational.
/// @param  r1   Left -hand operand.
/// @param  r2   Right-hand operand.
template <unsigned NB1, unsigned DB1, unsigned NB2, unsigned DB2>
constexpr bool operator<(rational<NB1, DB1> r1, rational<NB2, DB2> r2) {
  auto const c = common_denom(r1, r2);
  return c.n1 < c.n2;
}


/// Compare for greater-than-or-equal ordering with another rational.
/// @tparam NB1  Number of numerator  -bits in left -hand rational.
/// @tparam DB1  Number of denominator-bits in left -hand rational.
/// @tparam NB2  Number of numerator  -bits in right-hand rational.
/// @tparam DB2  Number of denominator-bits in right-hand rational.
/// @param  r1   Left -hand operand.
/// @param  r2   Right-hand operand.
template <unsigned NB1, unsigned DB1, unsigned NB2, unsigned DB2>
constexpr bool operator>=(rational<NB1, DB1> r1, rational<NB2, DB2> r2) {
  return !(r1 < r2);
}


/// Compare for greater-than ordering with another rational.
/// @tparam NB1  Number of numerator  -bits in left -hand rational.
/// @tparam DB1  Number of denominator-bits in left -hand rational.
/// @tparam NB2  Number of numerator  -bits in right-hand rational.
/// @tparam DB2  Number of denominator-bits in right-hand rational.
/// @param  r1   Left -hand operand.
/// @param  r2   Right-hand operand.
template <unsigned NB1, unsigned DB1, unsigned NB2, unsigned DB2>
constexpr bool operator>(rational<NB1, DB1> r1, rational<NB2, DB2> r2) {
  return !(r1 <= r2);
}


/// Copy rational number by way of unary operator+.
/// @tparam NB  Number of bits for numerator.
/// @tparam DB  Number of bits for denominator.
/// @param  r   Number to copy.
/// @return     Copy of number.
template <unsigned NB, unsigned DB>
constexpr auto operator+(rational<NB, DB> r) {
  return r;
}


/// Negate rational number.
/// @tparam NB  Number of bits for numerator.
/// @tparam DB  Number of bits for denominator.
/// @param  r   Number to negate.
/// @return     Negative of number.
template <unsigned NB, unsigned DB>
constexpr auto operator-(rational<NB, DB> r) {
  return rational<NB, DB>(-r.n(), r.d());
}


/// Sum of two rational numbers.
/// @tparam NB1  Number of numerator  -bits in left -hand rational.
/// @tparam DB1  Number of denominator-bits in left -hand rational.
/// @tparam NB2  Number of numerator  -bits in right-hand rational.
/// @tparam DB2  Number of denominator-bits in right-hand rational.
/// @param  r1   Addend.
/// @param  r2   Adder.
/// @return      Sum.
template <unsigned NB1, unsigned DB1, unsigned NB2, unsigned DB2>
constexpr auto operator+(rational<NB1, DB1> r1, rational<NB2, DB2> r2) {
  auto const c = common_denom(r1, r2);
  return rational<c.NMR_BITS, c.LCD_BITS>(c.n1 + c.n2, c.lcd);
}


// Modify rational number by adding other rational number.
template <unsigned NB, unsigned DB>
template <unsigned ONB, unsigned ODB>
constexpr rational<NB, DB> &rational<NB, DB>::
                            operator+=(rational<ONB, ODB> r) {
  return *this = *this + r;
}


/// Difference between two rational numbers.
/// @tparam NB1  Number of numerator  -bits in left -hand rational.
/// @tparam DB1  Number of denominator-bits in left -hand rational.
/// @tparam NB2  Number of numerator  -bits in right-hand rational.
/// @tparam DB2  Number of denominator-bits in right-hand rational.
/// @param  r1   Minuend.
/// @param  r2   Subtrahend.
/// @return      Difference.
template <unsigned NB1, unsigned DB1, unsigned NB2, unsigned DB2>
constexpr auto operator-(rational<NB1, DB1> r1, rational<NB2, DB2> r2) {
  return -r2 + r1;
}


// Modify this rational number by subtracting other rational number.
template <unsigned NB, unsigned DB>
template <unsigned ONB, unsigned ODB>
constexpr rational<NB, DB> &rational<NB, DB>::
                            operator-=(rational<ONB, ODB> r) {
  return *this = *this - r;
}


/// Product of two rational numbers.
/// @tparam NB1  Number of numerator  -bits in left -hand rational.
/// @tparam DB1  Number of denominator-bits in left -hand rational.
/// @tparam NB2  Number of numerator  -bits in right-hand rational.
/// @tparam DB2  Number of denominator-bits in right-hand rational.
/// @param  r1   Multiplicand.
/// @param  r2   Multiplier.
/// @return      Product.
template <unsigned NB1, unsigned DB1, unsigned NB2, unsigned DB2>
constexpr auto operator*(rational<NB1, DB1> r1, rational<NB2, DB2> r2) {
  auto const n1 = r1.n();
  auto const n2 = r2.n();
  auto const d1 = r1.d();
  auto const d2 = r2.d();
  auto const ga = gcd(n1, d2);
  auto const gb = gcd(n2, d1);
  enum { NB = (NB1 > NB2 ? NB1 : NB2), DB = (DB1 > DB2 ? DB1 : DB2) };
  using S = typename int_types<NB>::SF;
  using U = typename int_types<DB>::UF;
  return rational<NB, DB>(S(n1) / ga * n2 / gb, U(d1) / gb * d2 / ga);
}


/// Quotient of two rational numbers.
/// @tparam NB1  Number of numerator  -bits in left -hand rational.
/// @tparam DB1  Number of denominator-bits in left -hand rational.
/// @tparam NB2  Number of numerator  -bits in right-hand rational.
/// @tparam DB2  Number of denominator-bits in right-hand rational.
/// @param  r1   Dividend.
/// @param  r2   Divisor.
/// @return      Quotient.
template <unsigned NB1, unsigned DB1, unsigned NB2, unsigned DB2>
constexpr auto operator/(rational<NB1, DB1> r1, rational<NB2, DB2> r2) {
  return r1 * r2.reciprocal();
}


/// Quotient of two rational numbers.
/// @tparam NB1  Number of numerator  -bits in left -hand rational.
/// @tparam DB1  Number of denominator-bits in left -hand rational.
/// @param  r1   Dividend.
/// @param  r2   Divisor.
/// @return      Quotient.
template <unsigned NB1, unsigned DB1>
constexpr auto operator/(rational<NB1, DB1>                 r1,
                         typename rational<NB1, DB1>::stype r2) {
  return r1 * rational<NB1, DB1>(r2).reciprocal();
}


// Modify this rational number by multiplying by other rational number.
template <unsigned NB, unsigned DB>
template <unsigned ONB, unsigned ODB>
constexpr rational<NB, DB> &rational<NB, DB>::
                            operator*=(rational<ONB, ODB> r) {
  return *this = (*this) * r;
}


// Modify this rational number by dividing by other rational number.
template <unsigned NB, unsigned DB>
template <unsigned ONB, unsigned ODB>
constexpr rational<NB, DB> &rational<NB, DB>::
                            operator/=(rational<ONB, ODB> r) {
  return *this = (*this) / r;
}


/// Print rational number to output-stream.
/// @tparam NB  Number of bits for numerator   of rational.
/// @tparam DB  Number of bits for denominator of rational.
/// @param  s   Reference to output-stream.
/// @param  r   Rational number.
/// @return     Reference to modified output-stream.
template <unsigned NB, unsigned DB>
std::ostream &operator<<(std::ostream &s, rational<NB, DB> r) {
  s << int_fast64_t(r.n());
  if (r.d() != 1) { s << '/' << uint_fast64_t(r.d()); }
  return s;
}


} // namespace rat
} // namespace vnix

#endif // ndef VNIX_RAT_RATIONAL_HPP
