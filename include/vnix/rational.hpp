/// @file       rational.hpp
/// @brief      Definition of vnix::rational, vnix::common_denom.
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
/// @tparam U  Type of unsigned word in which rational number is encoded
template <typename U> class rational : public rat::encoding<U> {
  struct dummy_arg {};

  /// Construct from encoding.
  /// @param u  Encoding of rational number.
  constexpr rational(U u, dummy_arg) : rat::encoding<U>(u) {}

public:
  using typename rat::rational_base<U>::S;
  using rat::encoding<U>::n;
  using rat::encoding<U>::d;

  /// Initialize from numerator and denominator.
  /// @param n  Numerator   (zero  by default).
  /// @param d  Denominator (unity by default).
  constexpr rational(int64_t n = 0, int64_t d = 1)
      : rat::encoding<U>(rat::normalized_pair<U>(n, d)) {}

  /// Initialize from other rational.
  /// @tparam OU  Type of word in which other rational in encoded.
  /// @param  r   Copy of other rational.
  template <typename OU>
  constexpr rational(rational<OU> r)
      : rat::encoding<U>(rat::normalized_pair<U>(r.n(), r.d())) {}

  /// Convert to (signed) integer.
  constexpr S to_int() const {
    if (d() != 1) {
      throw "attempted conversion to integer from fraction";
    }
    return n();
  }

  /// Automatically convert to boolean.
  constexpr operator bool() const { return n() != 0; }

  /// Convert to double.
  constexpr double to_double() const { return n() * 1.0 / d(); }

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


/// Compare rationals for equality.
/// Promote both to uint64_t-storage for comparison.
/// @param r1  Left -hand operand.
/// @param r2  Right-hand operand.
constexpr bool operator==(rational<uint64_t> r1, rational<uint64_t> r2) {
  return r1.n() == r2.n() && r1.d() == r2.d();
}


/// Compare rationals for inequality.
/// Promote both to uint64_t-storage for comparison.
/// @param r1  Left -hand operand.
/// @param r2  Right-hand operand.
constexpr bool operator!=(rational<uint64_t> r1, rational<uint64_t> r2) {
  return r1.n() != r2.n() || r1.d() != r2.d();
}


/// Least common denominator (LCD) and numerators corresponding to a pair of
/// rational numbers.
struct common_denom {
  uint64_t const g;   ///< GCD of input denominators.
  uint64_t const d1g; ///< First  input denominator divided by g.
  uint64_t const d2g; ///< Second input denominator divided by g;
  uint64_t const d;   ///< LCD.
  int64_t const  n1;  ///< First  output numerator, corresponding to LCD.
  int64_t const  n2;  ///< Second output numerator, corresponding to LCD.

  /// Common denominator and associated numerators for rational pair.
  /// @tparam U   Type of unsigned integer word in which ration is encoded.
  /// @param  r1  First  input rational number.
  /// @param  r2  Second input rational number.
  template <typename U>
  constexpr common_denom(rational<U> r1, rational<U> r2)
      : g(gcd(r1.d(), r2.d())), //
        d1g(r1.d() / g),        //
        d2g(r2.d() / g),        //
        d(d1g * r2.d()),        //
        n1(r1.n() * d2g),       //
        n2(r2.n() * d1g) {}
};


/// Compare for ordering with another rational.
/// Promote both to uint64_t-storage for comparison.
/// @param r1  Left -hand operand.
/// @param r2  Right-hand operand.
constexpr bool operator<(rational<uint64_t> r1, rational<uint64_t> r2) {
  common_denom const c(r1, r2);
  return c.n1 < c.n2;
}


/// Copy rational number.
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
  common_denom const c(r1, r2);
  return {c.n1 + c.n2, c.d};
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
  return {n1 / ga * n2 / gb, d1 / gb * d2 / ga};
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


/// Print rational number to output-stream.
/// @tparam U  Type of unsigned word in which rational is encoded.
/// @param  s  Reference to output-stream.
/// @param  r  Rational number.
/// @return    Reference to modified output-stream.
template <typename U>
std::ostream &operator<<(std::ostream &s, rational<U> r) {
  return r.print(s);
}


/// Rational with five bits for numerator and three for denominator.
using rat8_t = rational<uint8_t>;

/// Rational with nine bits for numerator and seven for denominator.
using rat16_t = rational<uint16_t>;

/// Rational with 17 bits for numerator and 15 for denominator.
using rat32_t = rational<uint32_t>;

/// Rational with 33 bits for numerator and 31 for denominator.
using rat64_t = rational<uint64_t>;


} // namespace vnix

#endif // ndef VNIX_RATIONAL_HPP
