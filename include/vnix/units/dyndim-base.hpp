/// @file       units/dyndim-base.hpp
/// @brief      Definition of vnix::units::dyndim_base.
/// @copyright  2019  Thomas E. Vaughan
/// @license    GPL Version 3 or later.

#ifndef VNIX_UNITS_DYNDIM_BASE_HPP
#define VNIX_UNITS_DYNDIM_BASE_HPP

#include <vnix/units/dim.hpp>

namespace vnix {
namespace units {


/// Base-type for a dimensioned value whose dimension is specified, perhaps
/// dynamically at run-time, by way of the constructor.
///
/// The dimension is specified as a set of exponents, one for each of the five
/// base dimensions (time, length, mass, charge, and temperature).
///
/// For statdim_base, the dimension is always known statically at compile-time.
/// For dyndim_base, the dimension is known statically at compile-time only if
/// the dimension be specified to the constructor as a constant expression.
class dyndim_base {
  dim d_; ///< Exponents representing dimension.

public:
  /// Initialize from exponents representing dimension.
  constexpr dyndim_base(dim dd) : d_(dd) {}

  /// Exponent for each unit in dimensioned quantity.  This is not static
  /// because it needs to be consistent with signature of dyndim.
  constexpr dim d() const { return d_; }

  /// Throw if dimension be non-null.
  constexpr void number() const {
    if (d() != nul_dim) {
      throw "dimensioned quantity is not a number";
    }
  }

  /// Test for comparison of dimensioned values.
  /// @tparam B  dyndim_base or statdim_base.
  /// @param  b  Dimension of right side.
  template <typename B> constexpr void comparison(B const &b) const {
    if (d_ != b.d()) {
      throw "incompatible dimensions for comparison";
    }
  }

  /// Dimension for sum of dimensioned values.
  /// @tparam B  dyndim_base or statdim_base.
  /// @param  b  Dimension of addend.
  /// @return    Dimension of sum.
  template <typename B> constexpr dyndim_base sum(B const &b) const {
    if (d_ != b.d()) {
      throw "incompatible dimensions for addition";
    }
    return d_;
  }

  /// Dimension for difference of dimensioned values.
  /// @tparam B  dyndim_base or statdim_base.
  /// @param  b  Dimension of subtractor.
  /// @return    Dimension of difference.
  template <typename B> constexpr dyndim_base diff(B const &b) const {
    if (d_ != b.d()) {
      throw "incompatible dimensions for subtraction";
    }
    return d_;
  }

  /// Dimension for product of dimensioned values.
  /// @tparam B  dyndim_base or statdim_base.
  /// @param  b  Dimension of factor.
  /// @return    Dimension of product.
  template <typename B> constexpr dyndim_base prod(B const &b) const {
    return d_ + b.d();
  }

  /// Dimension for quotient of dimensioned values.
  /// @tparam B  dyndim_base or statdim_base.
  /// @param  b  Dimension of divisor.
  /// @return    Dimension of quotient.
  template <typename B> constexpr dyndim_base quot(B const &b) const {
    return d_ - b.d();
  }

  /// Dimension for reciprocal of dimensioned value.
  /// @return  Dimension of reciprocal.
  constexpr dyndim_base recip() const { return nul_dim - d_; }

  /// Dimension for rational power of dimensioned value.
  /// @tparam PN  Numerator   of power.
  /// @tparam PD  Denominator of power.
  /// @return     Dimension   of result.
  template <int64_t PN, int64_t PD = 1> constexpr dyndim_base pow() const {
    return d_ * rat(PN, PD);
  }

  /// Dimension for rational power of dimensioned value.
  /// @param  p  Rational power.
  /// @return    Dimension of result.
  constexpr dyndim_base pow(rat p) const { return d_ * p; }

  /// Dimension for square-root of dimensioned value.
  /// @return  Dimension of square-root.
  constexpr dyndim_base sqrt() const { return d_ / rat(2); }
};


} // namespace units
} // namespace vnix

#endif // ndef VNIX_UNITS_DYNDIM_BASE_HPP
