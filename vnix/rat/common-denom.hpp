/// @file       vnix/rat/common-denom.hpp
/// @brief      Definition of vnix::rat::common_denom.
/// @copyright  2019  Thomas E. Vaughan
/// @license    GPL Version 3 or later.

#ifndef VNIX_RAT_COMMON_DENOM_HPP
#define VNIX_RAT_COMMON_DENOM_HPP

#include <vnix/gcd.hpp>       // for gcd
#include <vnix/int-types.hpp> // for int_types

namespace vnix {
namespace rat {


template <unsigned NB, unsigned DB> class rational;


template <unsigned NB1, unsigned DB1, unsigned NB2, unsigned DB2>
struct common_denom_params;


/// Compute least common denominator and corresponding numerators for the
/// comparison of a pair of rational numbers.
///
/// @tparam NB1  Number of bits for numerator   of first  rational.
/// @tparam DB1  Number of bits for denominator of first  rational.
/// @tparam NB2  Number of bits for numerator   of second rational.
/// @tparam DB2  Number of bits for denominator of second rational.
/// @param  r1   First  input rational number.
/// @param  r2   Second input rational number.
/// @return      Instance of common_denom_pairs.
template <unsigned NB1, unsigned DB1, unsigned NB2, unsigned DB2>
constexpr common_denom_params<NB1, DB1, NB2, DB2>
common_denom(rational<NB1, DB1> r1, rational<NB2, DB2> r2) {
  using cdp       = common_denom_params<NB1, DB1, NB2, DB2>;
  using UF1       = typename int_types<DB1>::UF;
  using UF2       = typename int_types<DB2>::UF;
  using gcd_t     = gcd_promoted<UF1, UF2>;
  gcd_t const g   = gcd(r1.d(), r2.d()); // GCD of input denominators.
  UF1 const   d1g = r1.d() / g; // First input denominator divided by g.
  UF2 const   d2g = r2.d() / g; // Second input denominator divided by g.
  return cdp(d1g * r2.d(), r1.n() * d2g, r2.n() * d1g);
};


/// Least common denominator (LCD) and corresponding numerators for the
/// comparison of a pair of rational numbers.
///
/// An instance of common_denom_params is returned by common_denom().
///
/// @tparam NB1  Number of bits for numerator   of first  rational.
/// @tparam DB1  Number of bits for denominator of first  rational.
/// @tparam NB2  Number of bits for numerator   of second rational.
/// @tparam DB2  Number of bits for denominator of second rational.
template <unsigned NB1, unsigned DB1, unsigned NB2, unsigned DB2>
struct common_denom_params {
  /// Clamp an unsigned number at the maximum value of 64.
  /// @param n  Unsigned input.
  /// @return   Clamped output.
  constexpr static unsigned clamp64(unsigned n) { return n > 64 ? 64 : n; }

  /// Number of bits required for storage of various items.
  enum {
    LCD_BITS = clamp64(DB1 + DB2), ///< For storage of LCD.
    N1_BITS  = clamp64(NB1 + DB2), ///< For storage of first  numerator.
    N2_BITS  = clamp64(NB2 + DB1), ///< For storage of second numerator.
    /// For use by client for storage of sum of numerators.
    NMR_BITS = (N1_BITS > N2_BITS ? N1_BITS : N2_BITS)
  };

  using lcd_t = typename int_types<LCD_BITS>::UF; ///< Type of LCD.
  using n1_t  = typename int_types<N1_BITS>::SF;  ///< Type of first numerator.
  using n2_t  = typename int_types<N2_BITS>::SF;  ///< Type of secnd numerator.

  lcd_t const lcd; ///< LCD.
  n1_t const  n1;  ///< First  numerator.
  n2_t const  n2;  ///< Second numerator.

private:
  using rat1 = rational<NB1, DB1>; ///< Type of first  rational.
  using rat2 = rational<NB2, DB2>; ///< Type of second rational.

  /// Allow common_denom() to call private constructor.
  constexpr friend common_denom_params
  common_denom<NB1, DB1, NB2, DB2>(rat1 r1, rat2 r2);

  /// Construct from list of initializers.
  /// @param ll   Least common denominator.
  /// @param nn1  First  numerator.
  /// @param nn2  Second numerator.
  constexpr common_denom_params(lcd_t ll, n1_t nn1, n2_t nn2)
      : lcd(ll), n1(nn1), n2(nn2) {}
};


} // namespace rat
} // namespace vnix

#endif // ndef VNIX_RAT_COMMON_DENOM_HPP
