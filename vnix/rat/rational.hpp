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
/// @tparam U  Type of unsigned word in which rational number is encoded
template <typename U> class rational : public encoding<U> {
  struct dummy_arg {};

  /// Construct from encoding.
  /// @param u  Encoding of rational number.
  constexpr rational(U u, dummy_arg) : encoding<U>(u) {}

public:
  using typename rational_base<U>::S;
  using encoding<U>::n;
  using encoding<U>::d;

  /// Initialize from numerator and denominator.
  /// @param n  Numerator   (zero  by default).
  /// @param d  Denominator (unity by default).
  constexpr rational(int64_t n = 0, int64_t d = 1)
      : encoding<U>(normalized_pair<U>(n, d)) {}

  /// Initialize from other rational.
  /// @tparam OU  Type of word in which other rational in encoded.
  /// @param  r   Copy of other rational.
  template <typename OU>
  constexpr rational(rational<OU> r)
      : encoding<U>(normalized_pair<U>(r.n(), r.d())) {}

  /// Convert to (signed) integer.
  constexpr S to_int() const {
    if (d() != 1) { throw "attempted conversion to integer from fraction"; }
    return n();
  }

  /// Automatically convert to boolean.
  constexpr bool to_bool() const { return n() != 0; }

  /// Convert to double.
  constexpr double to_double() const { return n() * 1.0 / d(); }

  /// Modify this rational number by adding other rational number.
  /// @param r  Other rational number.
  /// @return   Reference to this rational number (after addition).
  constexpr rational &operator+=(rational r);

  /// Modify this rational number by adding other rational number.
  /// @param r  Other rational number (in this case, an integer).
  /// @return   Reference to this rational number (after addition).
  constexpr rational &operator+=(int64_t r);

  /// Modify this rational number by subtracting other rational number.
  /// @param r  Other rational number.
  /// @return   Reference to this rational number (after subtraction).
  constexpr rational &operator-=(rational r);

  /// Modify this rational number by subtracting other rational number.
  /// @param r  Other rational number (in this case, an integer).
  /// @return   Reference to this rational number (after subtraction).
  constexpr rational &operator-=(int64_t r);

  /// Reciprocal of this rational number.
  constexpr rational reciprocal() const {
    if (n() == 0) { throw "attempt to take reciprocal of zero"; }
    if (n() < 0) { return {-d(), -n()}; }
    return {d(), n()};
  }

  /// Modify this rational number by multiplying by other rational number.
  /// @param r  Other rational number.
  /// @return   Reference to this rational number (after multiplication).
  constexpr rational &operator*=(rational r);

  /// Modify this rational number by dividing by other rational number.
  /// @param r  Other rational number.
  /// @return   Reference to this rational number (after division).
  constexpr rational &operator/=(rational r);

  /// Modify this rational number by multiplying by other rational number.
  /// @param r  Other rational number (in this case, integer).
  /// @return   Reference to this rational number (after multiplication).
  constexpr rational &operator*=(int64_t r);

  /// Modify this rational number by dividing by other rational number.
  /// @param r  Other rational number (in this case, integer).
  /// @return   Reference to this rational number (after division).
  constexpr rational &operator/=(int64_t r);

  /// Encoding from rational number.
  constexpr static U encode(rational r) { return r.c_; }

  /// Rational number from encoding.
  constexpr static rational decode(U u) { return {u, dummy_arg()}; }
};


/// Compare rationals for equality.
/// Promote both to uint64_t-storage for comparison.
/// @param r1  Left -hand operand.
/// @param r2  Right-hand operand.
constexpr bool operator==(rational<uint64_t> r1, rational<uint64_t> r2) {
  return r1.n() == r2.n() && r1.d() == r2.d();
}


/// Compare rationals for equality.
/// Promote both to uint64_t-storage for comparison.
/// @param r1  Left -hand operand.
/// @param r2  Right-hand operand.
constexpr bool operator==(int64_t r1, rational<uint64_t> r2) {
  return rational<uint64_t>(r1) == r2;
}


/// Compare rationals for equality.
/// Promote both to uint64_t-storage for comparison.
/// @param r1  Left -hand operand.
/// @param r2  Right-hand operand.
constexpr bool operator==(rational<uint64_t> r1, int64_t r2) {
  return r1 == rational<uint64_t>(r2);
}


/// Compare rationals for inequality.
/// Promote both to uint64_t-storage for comparison.
/// @param r1  Left -hand operand.
/// @param r2  Right-hand operand.
constexpr bool operator!=(rational<uint64_t> r1, rational<uint64_t> r2) {
  return !(r1 == r2);
}


/// Compare rationals for inequality.
/// Promote both to uint64_t-storage for comparison.
/// @param r1  Left -hand operand.
/// @param r2  Right-hand operand.
constexpr bool operator!=(int64_t r1, rational<uint64_t> r2) {
  return !(r1 == r2);
}


/// Compare rationals for inequality.
/// Promote both to uint64_t-storage for comparison.
/// @param r1  Left -hand operand.
/// @param r2  Right-hand operand.
constexpr bool operator!=(rational<uint64_t> r1, int64_t r2) {
  return !(r1 == r2);
}


/// Compare for less-than-or-equal ordering with another rational.
/// Promote both to uint64_t-storage for comparison.
/// @param r1  Left -hand operand.
/// @param r2  Right-hand operand.
constexpr bool operator<=(rational<uint64_t> r1, rational<uint64_t> r2) {
  common_denom const c(r1, r2);
  return c.n1 <= c.n2;
}


/// Compare for less-than-or-equal ordering with another rational.
/// Promote both to uint64_t-storage for comparison.
/// @param r1  Left -hand operand.
/// @param r2  Right-hand operand.
constexpr bool operator<=(int64_t r1, rational<uint64_t> r2) {
  return rational<uint64_t>(r1) <= r2;
}


/// Compare for less-than-or-equal ordering with another rational.
/// Promote both to uint64_t-storage for comparison.
/// @param r1  Left -hand operand.
/// @param r2  Right-hand operand.
constexpr bool operator<=(rational<uint64_t> r1, int64_t r2) {
  return r1 <= rational<uint64_t>(r2);
}


/// Compare for less-than ordering with another rational.
/// Promote both to uint64_t-storage for comparison.
/// @param r1  Left -hand operand.
/// @param r2  Right-hand operand.
constexpr bool operator<(rational<uint64_t> r1, rational<uint64_t> r2) {
  rat::common_denom const c(r1, r2);
  return c.n1 < c.n2;
}


/// Compare for less-than ordering with another rational.
/// Promote both to uint64_t-storage for comparison.
/// @param r1  Left -hand operand.
/// @param r2  Right-hand operand.
constexpr bool operator<(int64_t r1, rational<uint64_t> r2) {
  return rational<uint64_t>(r1) < r2;
}


/// Compare for less-than ordering with another rational.
/// Promote both to uint64_t-storage for comparison.
/// @param r1  Left -hand operand.
/// @param r2  Right-hand operand.
constexpr bool operator<(rational<uint64_t> r1, int64_t r2) {
  return r1 < rational<uint64_t>(r2);
}


/// Compare for greater-than-or-equal ordering with another rational.
/// Promote both to uint64_t-storage for comparison.
/// @param r1  Left -hand operand.
/// @param r2  Right-hand operand.
constexpr bool operator>=(rational<uint64_t> r1, rational<uint64_t> r2) {
  common_denom const c(r1, r2);
  return c.n1 >= c.n2;
}


/// Compare for greater-than-or-equal ordering with another rational.
/// Promote both to uint64_t-storage for comparison.
/// @param r1  Left -hand operand.
/// @param r2  Right-hand operand.
constexpr bool operator>=(int64_t r1, rational<uint64_t> r2) {
  return rational<uint64_t>(r1) >= r2;
}


/// Compare for greater-than-or-equal ordering with another rational.
/// Promote both to uint64_t-storage for comparison.
/// @param r1  Left -hand operand.
/// @param r2  Right-hand operand.
constexpr bool operator>=(rational<uint64_t> r1, int64_t r2) {
  return r1 >= rational<uint64_t>(r2);
}


/// Compare for greater-than ordering with another rational.
/// Promote both to uint64_t-storage for comparison.
/// @param r1  Left -hand operand.
/// @param r2  Right-hand operand.
constexpr bool operator>(rational<uint64_t> r1, rational<uint64_t> r2) {
  rat::common_denom const c(r1, r2);
  return c.n1 > c.n2;
}


/// Compare for greater-than ordering with another rational.
/// Promote both to uint64_t-storage for comparison.
/// @param r1  Left -hand operand.
/// @param r2  Right-hand operand.
constexpr bool operator>(int64_t r1, rational<uint64_t> r2) {
  return rational<uint64_t>(r1) > r2;
}


/// Compare for greater-than ordering with another rational.
/// Promote both to uint64_t-storage for comparison.
/// @param r1  Left -hand operand.
/// @param r2  Right-hand operand.
constexpr bool operator>(rational<uint64_t> r1, int64_t r2) {
  return r1 > rational<uint64_t>(r2);
}


/// Copy rational number by way of unary operator+.
/// @tparam U  Type of unsigned word in which rational is encoded.
/// @param  r  Number to copy.
/// @return    Copy of number.
template <typename U> constexpr rational<U> operator+(rational<U> r) {
  return r;
}


/// Negate rational number.
/// @tparam U  Type of unsigned word in which rational is encoded.
/// @param  r  Number to negate.
/// @return    Negative of number.
template <typename U> constexpr rational<U> operator-(rational<U> r) {
  return {-r.n(), r.d()};
}


/// Sum of two rational numbers.
/// @tparam U   Type of unsigned word in which rational is encoded.
/// @param  r1  Addend.
/// @param  r2  Adder.
/// @return     Sum.
template <typename U>
constexpr rational<U> operator+(rational<U> r1, rational<U> r2) {
  rat::common_denom const c(r1, r2);
  return {c.n1 + c.n2, c.lcd};
}


/// Sum of two rational numbers.
/// @tparam U   Type of unsigned word in which rational is encoded.
/// @param  r1  Addend.
/// @param  r2  Adder.
/// @return     Sum.
template <typename U>
constexpr rational<U> operator+(int64_t r1, rational<U> r2) {
  return rational<U>(r1) + r2;
}


/// Sum of two rational numbers.
/// @tparam U   Type of unsigned word in which rational is encoded.
/// @param  r1  Addend.
/// @param  r2  Adder.
/// @return     Sum.
template <typename U>
constexpr rational<U> operator+(rational<U> r1, int64_t r2) {
  return r1 + rational<U>(r2);
}


// Modify rational number by adding other rational number.
template <typename U>
constexpr rational<U> &rational<U>::operator+=(rational<U> r) {
  return *this = *this + r;
}


// Modify rational number by adding other rational number.
template <typename U>
constexpr rational<U> &rational<U>::operator+=(int64_t r) {
  return *this = *this + r;
}


/// Difference between two rational numbers.
/// @tparam U   Type of unsigned word in which rational is encoded.
/// @param  r1  Minuend.
/// @param  r2  Subtrahend.
/// @return     Difference.
template <typename U>
constexpr rational<U> operator-(rational<U> r1, rational<U> r2) {
  return -r2 + r1;
}


/// Difference between two rational numbers.
/// @tparam U   Type of unsigned word in which rational is encoded.
/// @param  r1  Minuend.
/// @param  r2  Subtrahend.
/// @return     Difference.
template <typename U>
constexpr rational<U> operator-(int64_t r1, rational<U> r2) {
  return -r2 + r1;
}


/// Difference between two rational numbers.
/// @tparam U   Type of unsigned word in which rational is encoded.
/// @param  r1  Minuend.
/// @param  r2  Subtrahend.
/// @return     Difference.
template <typename U>
constexpr rational<U> operator-(rational<U> r1, int64_t r2) {
  return -r2 + r1;
}


// Modify this rational number by subtracting other rational number.
template <typename U>
constexpr rational<U> &rational<U>::operator-=(rational<U> r) {
  return *this = *this - r;
}


// Modify this rational number by subtracting other rational number.
template <typename U>
constexpr rational<U> &rational<U>::operator-=(int64_t r) {
  return *this = *this - r;
}


/// Product of two rational numbers.
/// @tparam U   Type of unsigned word in which rational is encoded.
/// @param  r1  Multiplicand.
/// @param  r2  Multiplier.
/// @return     Product.
template <typename U>
constexpr rational<U> operator*(rational<U> r1, rational<U> r2) {
  int64_t const  n1 = r1.n();
  int64_t const  n2 = r2.n();
  uint64_t const d1 = r1.d();
  uint64_t const d2 = r2.d();
  uint64_t const ga = gcd((n1 < 0 ? -n1 : n1), d2);
  uint64_t const gb = gcd((n2 < 0 ? -n2 : n2), d1);
  return {int64_t(n1 / ga * n2 / gb), int64_t(d1 / gb * d2 / ga)};
}


/// Product of two rational numbers.
/// @tparam U   Type of unsigned word in which rational is encoded.
/// @param  r1  Multiplicand.
/// @param  r2  Multiplier.
/// @return     Product.
template <typename U>
constexpr rational<U> operator*(int64_t r1, rational<U> r2) {
  return rational<U>(r1) * r2;
}


/// Product of two rational numbers.
/// @tparam U   Type of unsigned word in which rational is encoded.
/// @param  r1  Multiplicand.
/// @param  r2  Multiplier.
/// @return     Product.
template <typename U>
constexpr rational<U> operator*(rational<U> r1, int64_t r2) {
  return r1 * rational<U>(r2);
}


/// Quotient of two rational numbers.
/// @tparam U   Type of unsigned word in which rational is encoded.
/// @param  r1  Dividend.
/// @param  r2  Divisor.
/// @return     Quotient.
template <typename U>
constexpr rational<U> operator/(rational<U> r1, rational<U> r2) {
  return r1 * r2.reciprocal();
}


/// Quotient of two rational numbers.
/// @tparam U   Type of unsigned word in which rational is encoded.
/// @param  r1  Dividend.
/// @param  r2  Divisor.
/// @return     Quotient.
template <typename U>
constexpr rational<U> operator/(int64_t r1, rational<U> r2) {
  return r1 * r2.reciprocal();
}


/// Quotient of two rational numbers.
/// @tparam U   Type of unsigned word in which rational is encoded.
/// @param  r1  Dividend.
/// @param  r2  Divisor.
/// @return     Quotient.
template <typename U>
constexpr rational<U> operator/(rational<U> r1, int64_t r2) {
  return r1 * rational<U>(r2).reciprocal();
}


// Modify this rational number by multiplying by other rational number.
template <typename U>
constexpr rational<U> &rational<U>::operator*=(rational<U> r) {
  return *this = (*this) * r;
}


// Modify this rational number by dividing by other rational number.
template <typename U>
constexpr rational<U> &rational<U>::operator/=(rational<U> r) {
  return *this = (*this) / r;
}


// Modify this rational number by multiplying by other rational number.
template <typename U>
constexpr rational<U> &rational<U>::operator*=(int64_t r) {
  return *this = (*this) * r;
}


// Modify this rational number by dividing by other rational number.
template <typename U>
constexpr rational<U> &rational<U>::operator/=(int64_t r) {
  return *this = (*this) / r;
}


/// Print rational number to output-stream.
/// @tparam U  Type of unsigned word in which rational is encoded.
/// @param  s  Reference to output-stream.
/// @param  r  Rational number.
/// @return    Reference to modified output-stream.
template <typename U>
std::ostream &operator<<(std::ostream &s, rational<U> r) {
  s << int64_t(r.n());
  if (r.d() != 1) { s << '/' << uint64_t(r.d()); }
  return s;
}


} // namespace rat
} // namespace vnix

#endif // ndef VNIX_RAT_RATIONAL_HPP
