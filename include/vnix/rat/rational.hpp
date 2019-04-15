/// @file       rat/rational.hpp
/// @brief      Definition of vnix::rat::rational.
/// @copyright  2019  Thomas E. Vaughan
/// @license    GPL Version 3 or later.

#ifndef VNIX_RAT_RATIONAL_HPP
#define VNIX_RAT_RATIONAL_HPP

#include <vnix/rat/encoding.hpp>

namespace vnix {
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

  /// Encoding from rational number.
  constexpr static U encode(rational r) { return r.c_; }

  /// Rational number from encoding.
  constexpr static rational decode(U u) { return {u, dummy_arg()}; }
};


} // namespace rat
} // namespace vnix

#endif // ndef VNIX_RAT_RATIONAL_HPP
