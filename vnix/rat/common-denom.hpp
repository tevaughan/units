/// @file       vnix/rat/common-denom.hpp
/// @brief      Definition of vnix::rat::common_denom.
/// @copyright  2019  Thomas E. Vaughan
/// @license    GPL Version 3 or later.

#ifndef VNIX_RAT_COMMON_DENOM_HPP
#define VNIX_RAT_COMMON_DENOM_HPP

#include <vnix/gcd.hpp>

namespace vnix {
namespace rat {


template <typename U> class rational;


/// Least common denominator (LCD) and numerators corresponding to a pair of
/// rational numbers.
struct common_denom {
  uint64_t const g;   ///< GCD of input denominators.
  uint64_t const d1g; ///< First  input denominator divided by g.
  uint64_t const d2g; ///< Second input denominator divided by g;
  int64_t const  d;   ///< LCD (signed because den arg to rational is signed).
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


} // namespace rat
} // namespace vnix

#endif // ndef VNIX_RAT_COMMON_DENOM_HPP
