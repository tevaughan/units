/// @file       vnix/units/statdim-base.hpp
/// @brief      Definition of vnix::units::statdim_base.
/// @copyright  2019  Thomas E. Vaughan
/// @license    GPL Version 3 or later.

#ifndef VNIX_UNITS_STATDIM_BASE_HPP
#define VNIX_UNITS_STATDIM_BASE_HPP

#include <vnix/units/dim.hpp>

namespace vnix {
namespace units {


class dyndim_base;


/// Base-type for a dimensioned value whose dimension is specified statically
/// at compile-time by way of a template parameter.
///
/// The dimension is specified as a set of exponents, one for each of the five
/// base dimensions (time, length, mass, charge, and temperature).
///
/// For statdim_base, the dimension is always known statically at compile-time.
/// For dyndim_base, the dimension is known statically at compile-time only if
/// the dimension be specified to the constructor as a constant expression.
///
/// @tparam D  Encoding of dimensional exponents as a `uint64_t`.
template <uint64_t D> struct statdim_base {
  /// Check for compatibility on contruction from dim.
  /// @param dd  Candidate dimension.
  constexpr statdim_base(dim dd) {
    if (d() != dd) {
      throw "attempt to construct from incompatible dimension";
    }
  }

  constexpr statdim_base() {} ///< Allow default construction.

  /// Exponent for each unit in dimensioned quantity.
  constexpr static dim d() { return dim(D); }

  /// Throw if dimension be non-null.
  constexpr static void number() {
    if (d() != nul_dim) {
      throw "dimensioned quantity is not a number";
    }
  }

  /// Test for comparison of dimensioned values.
  constexpr static void comparison(statdim_base) {}

  /// Test for comparison of dimensioned values.
  constexpr static void comparison(dyndim_base const &db);

  /// Dimension for sum of dimensioned values.
  constexpr static auto sum(statdim_base) { return statdim_base(); }

  /// Dimension for sum of dimensioned values.
  constexpr static dyndim_base sum(dyndim_base const &db);

  /// Dimension for difference of dimensioned values.
  constexpr static auto diff(statdim_base) { return statdim_base(); }

  /// Dimension for difference of dimensioned values.
  constexpr static dyndim_base diff(dyndim_base const &db);

  /// Dimension for product of dimensioned values.
  /// @tparam OD  Encoding of factor's dimension in `uint64_t`.
  /// @return     Dimension of product.
  template <uint64_t OD> constexpr static auto prod(statdim_base<OD>) {
    uint64_t constexpr rd = d() + dim(OD);
    return statdim_base<rd>();
  }

  /// Dimension for product of dimensioned values.
  /// @param db  Factor's dimension.
  /// @return    Dimension of product.
  constexpr static dyndim_base prod(dyndim_base const &db);

  /// Dimension for quotient of dimensioned values.
  /// @tparam D  Encoding of divisor's dimension in `uint64_t`.
  /// @return    Dimension of quotient.
  template <uint64_t OD> constexpr static auto quot(statdim_base<OD>) {
    uint64_t constexpr rd = d() - dim(OD);
    return statdim_base<rd>();
  }

  /// Dimension for quotient of dimensioned values.
  /// @param db  Divisor's dimension.
  /// @return    Dimension of quotient.
  constexpr static dyndim_base quot(dyndim_base const &db);

  /// Base-dimensions corresponding to reciprocal of dimensioned quantity.
  using recip_basedim = statdim_base<uint64_t(nul_dim - d())>;

  /// Dimension for reciprocal of dimensioned value.
  /// @return  Dimension of reciprocal.
  constexpr static recip_basedim recip() { return recip_basedim(); }

  /// Dimension for rational power of dimensioned value.
  /// @tparam PN  Numerator   of power.
  /// @tparam PD  Denominator of power.
  /// @return     Dimension   of result.
  template <int64_t PN, int64_t PD = 1> constexpr static auto pow() {
    uint64_t constexpr rd = d() * rat(PN, PD);
    return statdim_base<rd>();
  }

  /// Dimension for rational power of dimensioned value.
  /// @param  p  Rational power.
  /// @return    Dimension of result.
  constexpr static dyndim_base pow(rat p);

  /// Dimension for square-root of dimensioned value.
  /// @return  Dimension of square-root.
  constexpr static auto sqrt() {
    uint64_t constexpr rd = d() / rat(2);
    return statdim_base<rd>();
  }
};


} // namespace units
} // namespace vnix


#include <vnix/units/dyndim-base.hpp>


namespace vnix {
namespace units {


// Test for comparison of dimensioned values.
template <uint64_t D>
constexpr void statdim_base<D>::comparison(dyndim_base const &db) {
  if (d() != db.d()) {
    throw "incompatible dimensions for comparison";
  }
}


// Dimension for sum of dimensioned values.
template <uint64_t D>
constexpr dyndim_base statdim_base<D>::sum(dyndim_base const &db) {
  if (d() != db.d()) {
    throw "incompatible dimensions for addition";
  }
  return d();
}


// Dimension for difference of dimensioned values.
template <uint64_t D>
constexpr dyndim_base statdim_base<D>::diff(dyndim_base const &db) {
  if (d() != db.d()) {
    throw "incompatible dimensions for subtraction";
  }
  return d();
}


// Dimension for product of dimensioned values.
template <uint64_t D>
constexpr dyndim_base statdim_base<D>::prod(dyndim_base const &db) {
  return d() + db.d();
}


// Dimension for quotient of dimensioned values.
template <uint64_t D>
constexpr dyndim_base statdim_base<D>::quot(dyndim_base const &db) {
  return d() - db.d();
}


// Dimension for rational power of dimensioned value.
template <uint64_t D> constexpr dyndim_base statdim_base<D>::pow(rat p) {
  return d() * p;
}


} // namespace units
} // namespace vnix

#endif // ndef VNIX_UNITS_STATDIM_BASE_HPP
