/// @file       units/statdim-base.hpp
/// @brief      Definition of vnix::units::statdim_base.
/// @copyright  2019 Thomas E. Vaughan
/// @license    GPL3.

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
  /// Although nothing is stored in statdim_base, allow constructor to accept
  /// a dummy dimension for uniformity with API of dyndim_base.
  constexpr statdim_base(dim = nul_dim) {}

  /// Exponent for each unit in dimensioned quantity.
  ///
  /// This is not declared static---even though there need be no reference to
  /// an instance---for consistency with signature of dyndim::d().
  constexpr dim d() const { return dim(D); }

  /// Dimension for sum of dimensioned values.
  constexpr auto sum(statdim_base) const { return statdim_base(); }

  /// Dimension for sum of dimensioned values.
  constexpr dyndim_base sum(dyndim_base const &db) const;

  /// Dimension for difference of dimensioned values.
  constexpr auto diff(statdim_base) const { return statdim_base(); }

  /// Dimension for difference of dimensioned values.
  constexpr dyndim_base diff(dyndim_base const &db) const;

  /// Dimension for product of dimensioned values.
  /// @tparam OD  Encoding of factor's dimension in `uint64_t`.
  /// @return     Dimension of product.
  template <uint64_t OD> constexpr auto prod(statdim_base<OD>) const {
    uint64_t constexpr rd = dim(D) + dim(OD);
    return statdim_base<rd>();
  }

  /// Dimension for product of dimensioned values.
  /// @param db  Factor's dimension.
  /// @return    Dimension of product.
  constexpr dyndim_base prod(dyndim_base const &db) const;

  /// Dimension for quotient of dimensioned values.
  /// @tparam D  Encoding of divisor's dimension in `uint64_t`.
  /// @return    Dimension of quotient.
  template <uint64_t OD> constexpr auto quot(statdim_base<OD>) const {
    uint64_t constexpr rd = dim(D) - dim(OD);
    return statdim_base<rd>();
  }

  /// Dimension for quotient of dimensioned values.
  /// @param db  Divisor's dimension.
  /// @return    Dimension of quotient.
  constexpr dyndim_base quot(dyndim_base const &db) const;

  /// Dimension for reciprocal of dimensioned value.
  /// @return  Dimension of reciprocal.
  constexpr auto recip() const {
    uint64_t constexpr rd = nul_dim - dim(D);
    return statdim_base<rd>();
  }

  /// Dimension for rational power of dimensioned value.
  /// @tparam PN  Numerator   of power.
  /// @tparam PD  Denominator of power.
  /// @return     Dimension   of result.
  template <int64_t PN, int64_t PD = 1> constexpr auto pow() const {
    uint64_t constexpr rd = dim(D) * rat(PN, PD);
    return statdim_base<rd>();
  }

  /// Dimension for rational power of dimensioned value.
  /// @param  p  Rational power.
  /// @return    Dimension of result.
  constexpr dyndim_base pow(rat p) const;

  /// Dimension for square-root of dimensioned value.
  /// @return  Dimension of square-root.
  constexpr auto sqrt() const {
    uint64_t constexpr rd = dim(D) / rat(2);
    return statdim_base<rd>();
  }
};


} // namespace units
} // namespace vnix


#include <vnix/units/dyndim-base.hpp>


namespace vnix {
namespace units {


// Dimension for sum of dimensioned values.
template <uint64_t D>
constexpr dyndim_base statdim_base<D>::sum(dyndim_base const &db) const {
  if (dim(D) != db.d()) {
    throw "incompatible dimensions for addition";
  }
  return dim(D);
}


// Dimension for difference of dimensioned values.
template <uint64_t D>
constexpr dyndim_base statdim_base<D>::diff(dyndim_base const &db) const {
  if (dim(D) != db.d()) {
    throw "incompatible dimensions for subtraction";
  }
  return dim(D);
}


// Dimension for product of dimensioned values.
template <uint64_t D>
constexpr dyndim_base statdim_base<D>::prod(dyndim_base const &db) const {
  return dim(D) + db.d();
}


// Dimension for quotient of dimensioned values.
template <uint64_t D>
constexpr dyndim_base statdim_base<D>::quot(dyndim_base const &db) const {
  return dim(D) - db.d();
}


// Dimension for rational power of dimensioned value.
template <uint64_t D> constexpr dyndim_base statdim_base<D>::pow(rat p) const {
  return dim(D) * p;
}


} // namespace units
} // namespace vnix

#endif // ndef VNIX_UNITS_STATDIM_BASE_HPP

