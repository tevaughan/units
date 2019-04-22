/// @file       vnix/rat/normalized-pair.hpp
/// @brief      Definition of vnix::rat::normalized_pair.
/// @copyright  2019 Thomas E. Vaughan
/// @license    GPL Version 3 or later.

#ifndef VNIX_RAT_NORMALIZED_PAIR_HPP
#define VNIX_RAT_NORMALIZED_PAIR_HPP

#include <vnix/gcd.hpp>
#include <vnix/rat/rational-base.hpp>

namespace vnix {
namespace rat {


/// Numerator and denominator of a rational number as separate numbers, not
/// encoded into the same word.
///
/// These separate numbers are guaranteed to be normalized so that they can be
/// encoded into a word of type rational_base<NMR_BITS, DNM_BITS>::US.
///
/// @tparam NMR_BITS  Number of bits for numerator.
/// @tparam DNM_BITS  Number of bits for denominator.
template <unsigned NMR_BITS, unsigned DNM_BITS>
class normalized_pair : rational_base<NMR_BITS, DNM_BITS> {
  using P = rational_base<NMR_BITS, DNM_BITS>; ///< Type of parent.
  using typename P::UF;
  using typename P::SF;
  UF g_; ///< Greatest common divisor for initial numer and denom.
  SF n_; ///< Normalized numerator.
  UF d_; ///< Normalized denominator.

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
  constexpr normalized_pair(SF n, SF d)
      : g_(gcd(n, d)),                //
        n_(d < 0 ? -n / g_ : n / g_), //
        d_(d < 0 ? -d / g_ : d / g_) {
    if (d == 0) {
      throw "null denominator"; // Do not allow division by zero.
    }
    enum { NMAX = UF(1) << (NMR_BITS - 1) };
    if (n_ >= NMAX) { throw "numerator too large and positive"; }
    if (n_ < -NMAX) { throw "numerator too large and negative"; }
    if (d_ > (UF(1) << DNM_BITS)) { throw "denominator too large"; }
  }

  constexpr UF g() const { return g_; } ///< GCD.
  constexpr SF n() const { return n_; } ///< Normalized numerator.
  constexpr UF d() const { return d_; } ///< Normalized denominator.
};


} // namespace rat
} // namespace vnix

#endif // ndef VNIX_RAT_NORMALIZED_PAIR_HPP
