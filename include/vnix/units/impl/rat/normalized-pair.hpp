/// @file       units/impl/rat/normalized-pair.hpp
/// @brief      Definition of vnix::units::impl::rat::normalized_pair.
/// @copyright  2019 Thomas E. Vaughan
/// @license    GPL3.

#ifndef VNIX_UNITS_IMPL_RAT_NORMALIZED_PAIR_HPP
#define VNIX_UNITS_IMPL_RAT_NORMALIZED_PAIR_HPP

#include <vnix/units/impl/rat/gcd.hpp>
#include <vnix/units/impl/rat/rational-base.hpp>

namespace vnix {
namespace units {
namespace impl {
namespace rat {


/// Numerator and denominator of a rational number as separate numbers, not
/// encoded into the same word.
///
/// These separate numbers are guaranteed to be normalized so that they can be
/// encoded into a word of type U.
///
/// @tparam U  Type of unsigned word in which rational number will be encoded.
template <typename U> class normalized_pair : rational_base<U> {
  using typename rational_base<U>::S;
  using rational_base<U>::NMR_BITS;
  using rational_base<U>::DNM_BITS;

  int64_t g_;  ///< Greatest common divisor for initial numer and denom.
  S       n_;  ///< Normalized numerator.
  U       d_;  ///< Normalized denominator.

  /// Absolute value.
  /// @param x  Positive or negative value.
  /// @return   Corresponding positive value.
  constexpr static int64_t abs(int64_t x) { return x > 0 ? x : -x; }

public:
  /// Initialize normalized numerator and denominator for encoding of rational
  /// number.
  ///
  /// Normalization includes making sure that the denominator is positive,
  /// that the numerator and denominator are relatively prime, and that each
  /// fits in its allocation of bits within the word.
  ///
  /// @param n  Initial numerator.
  /// @param d  Initial denominator.
  constexpr normalized_pair(int64_t n, int64_t d)
      : g_(gcd(abs(n), abs(d))),       //
        n_(d >= 0 ? n / g_ : -n / g_), //
        d_(d >= 0 ? d / g_ : -d / g_) {
    if (d == 0) {
      throw "null denominator"; // Do not allow division by zero.
    }
    enum { NMAX = 1 << (NMR_BITS - 1) };
    if (n_ >= NMAX) {
      throw "numerator too large and positive";
    }
    if (n_ < -NMAX) {
      throw "numerator too large and negative";
    }
    if (d_ > (1 << DNM_BITS)) {
      throw "denominator too large";
    }
  }

  constexpr S n() const { return n_; } ///< Normalized numerator.
  constexpr U d() const { return d_; } ///< Normalized denominator.
};


} // namespace rat
} // namespace impl
} // namespace units
} // namespace vnix

#endif // ndef VNIX_UNITS_IMPL_RAT_NORMALIZED_PAIR_HPP
