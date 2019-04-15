/// @file       rational.hpp
/// @brief      Definition of operators and typedefs for rat::rational.
/// @copyright  2019  Thomas E. Vaughan
/// @license    GPL Version 3 or later.

#ifndef VNIX_RATIONAL_HPP
#define VNIX_RATIONAL_HPP

#include <iostream>
#include <vnix/rat/common-denom.hpp>
#include <vnix/rat/rational.hpp>

namespace vnix {


/// Compare rationals for equality.
/// Promote both to uint64_t-storage for comparison.
/// @param r1  Left -hand operand.
/// @param r2  Right-hand operand.
constexpr bool operator==(rat::rational<uint64_t> r1,
                          rat::rational<uint64_t> r2) {
  return r1.n() == r2.n() && r1.d() == r2.d();
}


/// Compare rationals for inequality.
/// Promote both to uint64_t-storage for comparison.
/// @param r1  Left -hand operand.
/// @param r2  Right-hand operand.
constexpr bool operator!=(rat::rational<uint64_t> r1,
                          rat::rational<uint64_t> r2) {
  return r1.n() != r2.n() || r1.d() != r2.d();
}


/// Compare for ordering with another rational.
/// Promote both to uint64_t-storage for comparison.
/// @param r1  Left -hand operand.
/// @param r2  Right-hand operand.
constexpr bool operator<(rat::rational<uint64_t> r1,
                         rat::rational<uint64_t> r2) {
  rat::common_denom const c(r1, r2);
  return c.n1 < c.n2;
}


/// Copy rational number.
/// @tparam U  Type of unsigned word in which rational is encoded.
/// @param  r  Number to copy.
/// @return    Copy of number.
template <typename U>
constexpr rat::rational<U> operator+(rat::rational<U> r) {
  return r;
}


/// Negate rational number.
/// @tparam U  Type of unsigned word in which rational is encoded.
/// @param  r  Number to negate.
/// @return    Negative of number.
template <typename U>
constexpr rat::rational<U> operator-(rat::rational<U> r) {
  return {-r.n(), r.d()};
}


/// Sum of two rational numbers.
/// @tparam U   Type of unsigned word in which rational is encoded.
/// @param  r1  Addend.
/// @param  r2  Adder.
/// @return     Sum.
template <typename U>
constexpr rat::rational<U> operator+(rat::rational<U> r1,
                                     rat::rational<U> r2) {
  rat::common_denom const c(r1, r2);
  return {c.n1 + c.n2, c.d};
}


/// Difference between two rational numbers.
/// @tparam U   Type of unsigned word in which rational is encoded.
/// @param  r1  Minuend.
/// @param  r2  Subtrahend.
/// @return     Difference.
template <typename U>
constexpr rat::rational<U> operator-(rat::rational<U> r1,
                                     rat::rational<U> r2) {
  return -r2 + r1;
}


/// Product of two rational numbers.
/// @tparam U   Type of unsigned word in which rational is encoded.
/// @param  r1  Multiplicand.
/// @param  r2  Multiplier.
/// @return     Product.
template <typename U>
constexpr rat::rational<U> operator*(rat::rational<U> r1,
                                     rat::rational<U> r2) {
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
constexpr rat::rational<U> operator/(rat::rational<U> r1,
                                     rat::rational<U> r2) {
  return r1 * r2.reciprocal();
}


/// Print rational number to output-stream.
/// @tparam U  Type of unsigned word in which rational is encoded.
/// @param  s  Reference to output-stream.
/// @param  r  Rational number.
/// @return    Reference to modified output-stream.
template <typename U>
std::ostream &operator<<(std::ostream &s, rat::rational<U> r) {
  if (r.d() != 1) {
    s << "[";
  }
  s << int64_t(r.n());
  if (r.d() != 1) {
    s << '/' << uint64_t(r.d()) << "]";
  }
  return s;
}


/// Rational with five bits for numerator and three for denominator.
using rat8_t = rat::rational<uint8_t>;

/// Rational with nine bits for numerator and seven for denominator.
using rat16_t = rat::rational<uint16_t>;

/// Rational with 17 bits for numerator and 15 for denominator.
using rat32_t = rat::rational<uint32_t>;

/// Rational with 33 bits for numerator and 31 for denominator.
using rat64_t = rat::rational<uint64_t>;


} // namespace vnix

#endif // ndef VNIX_RATIONAL_HPP
