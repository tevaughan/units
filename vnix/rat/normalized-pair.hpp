/// @file       vnix/rat/normalized-pair.hpp
/// @brief      Definition of vnix::rat::normalized_pair.
/// @copyright  2019 Thomas E. Vaughan
/// @license    GPL Version 3 or later.

#ifndef VNIX_RAT_NORMALIZED_PAIR_HPP
#define VNIX_RAT_NORMALIZED_PAIR_HPP

#include <utility>            // for pair
#include <vnix/gcd.hpp>       // for gcd
#include <vnix/int-types.hpp> // for int_types

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
template <unsigned NMR_BITS, unsigned DNM_BITS> class normalized_pair {
  // Each type used here is big enough to hold both the numerator and the
  // denominator of the associated rational number.  This will work even when
  // no bits are assigned to the denominator.  Otherwise, there are extra bits
  // that can handle a pair that are initially far from being relatively prime.
  using types = int_types<NMR_BITS + DNM_BITS>; ///< Working integer-types.
  using S     = typename types::SF; ///< Fastest large-enough   signed type.
  using U     = typename types::UF; ///< Fastest large-enough unsigned type.

  std::pair<S, U> pair_; ///< Normalized numerator and denominator.

  /// Normalize numerator and denominator.
  /// @param n  Input numerator.
  /// @param d  Input denominator.
  /// @return   Normalized numerator and denominator.
  constexpr static std::pair<S, U> pair(S n, S d) {
    U const g = gcd(n, d);
    if (d < 0) { return {-n / g, -d / g}; }
    return {n / g, d / g};
  }

public:
  /// Initialize normalized numerator and denominator for encoding of rational
  /// number.
  ///
  /// Normalization includes making sure that the denominator is positive,
  /// that the numerator and denominator are relatively prime, and that each
  /// fits in its allocation of bits within the word.
  ///
  /// @param nn  Initial numerator.
  /// @param dd  Initial denominator.
  constexpr normalized_pair(S nn, S dd) : pair_(pair(nn, dd)) {
    enum {
      NMAX = U(1) << (NMR_BITS - 1), // maximum magnitude of numerator
      DMAX = U(1) << (DNM_BITS)      // maximum value of denominator
    };
    if (dd == 0) { throw "null denominator (division by zero)"; }
    if (n() >= NMAX) { throw "numerator too large and positive"; }
    if (n() < -NMAX) { throw "numerator too large and negative"; }
    if (d() > +DMAX) { throw "denominator too large"; }
  }

  constexpr S n() const { return pair_.first; }  ///< Normalized numerator.
  constexpr U d() const { return pair_.second; } ///< Normalized denominator.
};


} // namespace rat
} // namespace vnix

#endif // ndef VNIX_RAT_NORMALIZED_PAIR_HPP
