/// @file       units/dimension.hpp
/// @brief      Definition of vnix::units::dimension.
/// @copyright  2019 Thomas E. Vaughan
/// @license    GPL3.

#ifndef VNIX_UNITS_DIMENSION_HPP
#define VNIX_UNITS_DIMENSION_HPP

#include <vnix/units/impl/dim-base.hpp> // for dim_base

namespace vnix {
namespace units {


/// Exponents, one for each fundamental unit of a dimensioned quantity.
struct dimension : protected impl::dim_base {
  using dim_base::dim_base;                    ///< Inherit constructor.
  constexpr float m() const { return get(M); } ///< Exponent for meters.
  constexpr float k() const { return get(K); } ///< Exponent for kilograms.
  constexpr float s() const { return get(S); } ///< Exponent for seconds.
  constexpr float c() const { return get(C); } ///< Exponent for Coulombs.
  constexpr float t() const { return get(T); } ///< Exponent for Kelvins.
  /// True only if exponent-sets be identical.
  constexpr bool operator==(dimension d) const { return e_ == d.e_; }
  /// True only if exponent-sets be different.
  constexpr bool operator!=(dimension d) const { return e_ != d.e_; }
  /// Set of exponents, each of which is sum of exponent in current instance
  /// and exponent in other set.
  /// @param   Exponents in other set.
  /// @return  Sum of this and other.
  constexpr dimension operator+(dimension d) const {
    float const nm = m() + d.m();
    float const nk = k() + d.k();
    float const ns = s() + d.s();
    float const nc = c() + d.c();
    float const nt = t() + d.t();
    return dimension(nm, nk, ns, nc, nt);
  }
  /// Set of exponents, each of which is difference between exponent in current
  /// instance and exponent in other set.
  /// @param   Exponents in other set.
  /// @return  Difference between this and other.
  constexpr dimension operator-(dimension d) const {
    float const nm = m() - d.m();
    float const nk = k() - d.k();
    float const ns = s() - d.s();
    float const nc = c() - d.c();
    float const nt = t() - d.t();
    return dimension(nm, nk, ns, nc, nt);
  }
  /// Modify current instance by adding each exponent from other set of
  /// exponents to corresponding exponent here.
  /// @param   Exponents in other set.
  /// @return  Reference to present instance after accumulation.
  constexpr dimension &operator+=(dimension d) { return *this = *this + d; }
  /// Modify current instance by subtracting from each exponent the
  /// corresponding exponent in another set of exponents.
  /// @param   Exponents in other set.
  /// @return  Reference to present instance after decrement.
  constexpr dimension &operator-=(dimension d) { return *this = *this - d; }
  /// Unary-plus as identity-copy operator.
  friend constexpr dimension operator+(dimension d) { return d; }
  /// Unary-minus as negation operator.
  friend constexpr dimension operator-(dimension d) {
    return dimension(-d.m(), -d.k(), -d.s(), -d.c(), -d.t());
  }
};


/// Instance corresponding to dimensionless dimval that can be converted to
/// double without throwing an exception.
constexpr dimension null{0, 0, 0, 0, 0};


} // namespace units
} // namespace vnix

#endif // ndef VNIX_DIMENSION_HPP
