/// @file       units/dimval.hpp
/// @brief      Definition of vnix::units::dimval, descendants, and typedefs.
/// @copyright  2019  Thomas E. Vaughan
/// @license    GPL Version 3 or later.

#ifndef VNIX_UNITS_DIMVAL_HPP
#define VNIX_UNITS_DIMVAL_HPP

#include <cmath>                       // for sqrt
#include <vnix/units/dim.hpp>          // for dim
#include <vnix/units/dyndim-base.hpp>  // for dyndim_base
#include <vnix/units/number.hpp>       // for number
#include <vnix/units/statdim-base.hpp> // for statdim_base

namespace vnix {

/// Classes and functions supporting the implementation of a model of
/// physically dimensioned quantities.
namespace units {


template <typename T, typename B> class dimval;
template <typename T> class basic_dyndim;
template <uint64_t D, typename T> class basic_statdim;

// Forward declaration for friend of dimval.
template <typename T, typename B>
constexpr auto operator/(double d, dimval<T, B> const &v);

// Forward declaration for friend of dimval.
template <typename T, typename B>
constexpr auto operator/(int d, dimval<T, B> const &v);


/// Model of a physically dimensioned quantity.
/// @tparam T  Type of storage (e.g., float or double) for numerical quantity.
/// @tparam B  Base-class (statdim_base or dyndim_base) for dimension.
template <typename T, typename B> class dimval : public number<T>, public B {
  /// Allow access to every kind of dimval.
  /// @tparam OT  Type of other dimval's numeric value.
  /// @tparam OB  Type of other dimval's base-class for dimension.
  template <typename OT, typename OB> friend class dimval;

  /// Allow access to each kind (float or double) of dyndim.
  /// @tparam OT  Type of dyndim's numeric value.
  template <typename OT> friend class basic_dyndim;

  /// Allow access to every kind of statdim.
  /// @tparam D   Encoding of dimension in `uint64_t`.
  /// @tparam OT  Type of statdim's numeric value.
  template <uint64_t D, typename OT> friend class basic_statdim;

  /// Allow access to every kind of division-operator with number on left.
  template <typename OT, typename OB>
  friend constexpr auto operator/(double d, dimval<OT, OB> const &v);

  /// Allow access to every kind of division-operator with number on left.
  template <typename OT, typename OB>
  friend constexpr auto operator/(int d, dimval<OT, OB> const &v);

protected:
  /// Initialize from numeric value and from dimension.
  /// Constructor takes double, regardless of what T is.
  /// @param v  Numeric value.
  /// @param d  Dimension.
  constexpr dimval(double v, dim const &d) : number<T>(v), B(d) {}

  using number<T>::v_; ///< Allow access to numeric value.

public:
  using B::d; ///< Allow access to dimension.

  /// Initialize from other dimensioned value.
  /// @tparam OT  Numeric type of other dimensioned value.
  /// @tparam OB  Base-dimension type of other dimensioned value.
  /// @param  v   Reference to other dimensioned value.
  template <typename OT, typename OB>
  constexpr dimval(dimval<OT, OB> const &v) : number<T>(v.v_), B(v.d()) {}

  /// Initialize from dimensionless number.
  /// @param n  Number.
  constexpr dimval(double n) : number<T>(n), B(nul_dim) {}

  /// Initialize from dimensionless number.
  /// @param n  Number.
  constexpr dimval(int n) : number<T>(n), B(nul_dim) {}

  /// Convert to dimensionless number.
  constexpr operator double() const {
    B::number();
    return v_;
  }

  /// Exponent for base at specified offset.
  /// @param off  Offset.
  constexpr rat d(base_off off) const { return B::d()[off]; }

  /// Equality-comparison of two dimensioned values.
  /// This will throw an exception if the dimensions are different.
  /// @tparam OT  Numeric type of other dimensioned value.
  /// @tparam OB  Base-dimension type of other dimensioned value.
  /// @param  v   Reference to other dimensioned value.
  /// @return     True only if this and the other be equal.
  template <typename OT, typename OB>
  constexpr auto operator==(dimval<OT, OB> const &v) const {
    B::comparison(v); // Check for compatibility of units.
    return v_ == v.v_;
  }

  /// Inequality-comparison of two dimensioned values.
  /// This will throw an exception if the dimensions are different.
  /// @tparam OT  Numeric type of other dimensioned value.
  /// @tparam OB  Base-dimension type of other dimensioned value.
  /// @param  v   Reference to other dimensioned value.
  /// @return     True only if this and the other be unequal.
  template <typename OT, typename OB>
  constexpr auto operator!=(dimval<OT, OB> const &v) const {
    return !(*this == v);
  }

  /// Less-than comparison of two dimensioned values.
  /// This will throw an exception if the dimensions are different.
  /// @tparam OT  Numeric type of other dimensioned value.
  /// @tparam OB  Base-dimension type of other dimensioned value.
  /// @param  v   Reference to other dimensioned value.
  /// @return     True only if this be less than the other.
  template <typename OT, typename OB>
  constexpr auto operator<(dimval<OT, OB> const &v) const {
    B::comparison(v); // Check for compatibility of units.
    return v_ < v.v_;
  }

  /// Less-than-or-equal comparison of two dimensioned values.
  /// This will throw an exception if the dimensions are different.
  /// @tparam OT  Numeric type of other dimensioned value.
  /// @tparam OB  Base-dimension type of other dimensioned value.
  /// @param  v   Reference to other dimensioned value.
  /// @return     True only if this be less than or equal to the other.
  template <typename OT, typename OB>
  constexpr auto operator<=(dimval<OT, OB> const &v) const {
    B::comparison(v); // Check for compatibility of units.
    return v_ <= v.v_;
  }

  /// Greater-than comparison of two dimensioned values.
  /// This will throw an exception if the dimensions are different.
  /// @tparam OT  Numeric type of other dimensioned value.
  /// @tparam OB  Base-dimension type of other dimensioned value.
  /// @param  v   Reference to other dimensioned value.
  /// @return     True only if this be greater than the other.
  template <typename OT, typename OB>
  constexpr auto operator>(dimval<OT, OB> const &v) const {
    return !(*this <= v);
  }

  /// Greater-than-or-equal comparison of two dimensioned values.
  /// This will throw an exception if the dimensions are different.
  /// @tparam OT  Numeric type of other dimensioned value.
  /// @tparam OB  Base-dimension type of other dimensioned value.
  /// @param  v   Reference to other dimensioned value.
  /// @return     True only if this be greater than or equal to the other.
  template <typename OT, typename OB>
  constexpr auto operator>=(dimval<OT, OB> const &v) const {
    return !(*this < v);
  }

  /// Sum of two dimensioned values.
  /// @tparam OT  Numeric type of addend.
  /// @tparam OB  Base-dimension type of addend.
  /// @param  v   Addend.
  /// @return     Sum.
  template <typename OT, typename OB>
  constexpr auto operator+(dimval<OT, OB> const &v) const {
    auto const sdim = B::sum(v); // Check for compatibility of units.
    return dimval(v_ + v.v_, sdim.d());
  }

  /// Difference between two dimensioned values.
  /// @tparam OT  Numeric type of subtractor.
  /// @tparam OB  Base-dimension type of subtractor.
  /// @param  v   Subractor.
  /// @return     Difference.
  template <typename OT, typename OB>
  constexpr auto operator-(dimval<OT, OB> const &v) const {
    auto const ddim = B::diff(v); // Check for compatibility of units.
    return dimval(v_ - v.v_, ddim.d());
  }

  /// Modify present instance by adding in a dimensioned value.
  /// @tparam OT  Numeric type of addend.
  /// @tparam OB  Base-dimension type of addend.
  /// @param  v   Addend.
  /// @return     Sum.
  template <typename OT, typename OB>
  constexpr dimval &operator+=(dimval<OT, OB> const &v) {
    B::sum(v); // Check for compatibility of units.
    v_ += v.v_;
    return *this;
  }

  /// Modify present instance by subtracting out a dimensioned value.
  /// @tparam OT  Numeric type of subtractor.
  /// @tparam OB  Base-dimension type of subtractor.
  /// @param  v   Subtractor.
  /// @return     Difference.
  template <typename OT, typename OB>
  constexpr dimval &operator-=(dimval<OT, OB> const &v) {
    B::diff(v); // Check for compatibility of units.
    v_ -= v.v_;
    return *this;
  }

  /// Scale dimensioned value.
  /// @param n  Scale-factor.
  /// @return   Scaled value.
  constexpr dimval operator*(double n) const { return {v_ * n, d()}; }

  /// Scale dimensioned value.
  /// @param n  Scale-factor.
  /// @return   Scaled value.
  constexpr dimval operator*(int n) const { return {v_ * n, d()}; }

  /// Scale dimensioned value.
  /// @param n  Scale-factor.
  /// @param v  Original value.
  /// @return   Scaled value.
  friend constexpr dimval operator*(double n, dimval const &v) {
    return v * n;
  }

  /// Scale dimensioned value.
  /// @param n  Scale-factor.
  /// @param v  Original value.
  /// @return   Scaled value.
  friend constexpr dimval operator*(int n, dimval const &v) { return v * n; }

  /// Scale dimensioned quantity by dividing by number.
  constexpr dimval operator/(double n) const { return {v_ / n, d()}; }

  /// Scale dimensioned quantity by dividing by number.
  constexpr dimval operator/(int n) const { return {v_ / n, d()}; }

  /// Multiply two dimensioned values.
  /// @tparam OT  Numeric type of factor.
  /// @tparam OB  Base-dimension type of factor.
  /// @param  v   Factor.
  /// @return     Product.
  template <typename OT, typename OB>
  constexpr auto operator*(dimval<OT, OB> const &v) const {
    auto const pdim = B::prod(v);
    return dimval<T, decltype(pdim)>(v_ * v.v_, pdim.d());
  }

  /// Divide two dimensioned values.
  /// @tparam OT  Numeric type of divisor.
  /// @tparam OB  Base-dimension type of divisor.
  /// @param  v   Divisor.
  /// @return     Quotient.
  template <typename OT, typename OB>
  constexpr auto operator/(dimval<OT, OB> const &v) const {
    auto const qdim = B::quot(v);
    return dimval<T, decltype(qdim)>(v_ / v.v_, qdim.d());
  }

  /// Modify present instance by multiplying in a dimensionless value.
  /// @param v  Dimensionless scale-factor.
  /// @return   Scaled value.
  constexpr dimval &operator*=(double v) {
    v_ *= v;
    return *this;
  }

  /// Modify present instance by multiplying in a dimensionless value.
  /// @param v  Dimensionless scale-factor.
  /// @return   Scaled value.
  constexpr dimval &operator*=(int v) {
    v_ *= v;
    return *this;
  }

  /// Modify present instance by dividing it by a dimensionless value.
  /// @param v  Dimensionless scale-divisor.
  /// @return   Scaled value.
  constexpr dimval &operator/=(double v) {
    v_ /= v;
    return *this;
  }

  /// Modify present instance by dividing it by a dimensionless value.
  /// @param v  Dimensionless scale-divisor.
  /// @return   Scaled value.
  constexpr dimval &operator/=(int v) {
    v_ /= v;
    return *this;
  }

  /// Raise dimensioned value to rational power.
  /// @tparam PN  Numerator of power.
  /// @tparam PD  Denominator of power (by default, 1).
  /// @return     Transformed value of different dimension.
  template <int64_t PN, int64_t PD = 1> constexpr auto pow() const {
    auto const pdim = B::template pow<PN, PD>();
    return dimval<T, decltype(pdim)>(std::pow(v_, PN * 1.0 / PD), pdim.d());
  }

  /// Raise dimensioned value to rational power.
  /// @param p  Rational power.
  /// @return   Transformed value of different dimension.
  constexpr auto pow(rat p) const {
    auto const pdim = B::pow(p);
    using rt        = dimval<T, decltype(pdim)>;
    return rt(std::pow(v_, p.to_double()), pdim.d());
  }

  /// Raise dimensioned value to rational power.
  /// @param v  Original dimensioned value.
  /// @param p  Rational power.
  /// @return   Transformed value of different dimension.
  constexpr friend auto pow(dimval const &v, rat p) { return v.pow(p); }

  /// Take the squre root of a dimensioned quantity.
  constexpr auto sqrt() const {
    auto const rdim = B::sqrt();
    return dimval<T, decltype(rdim)>(std::sqrt(v_), rdim.d());
  }

  /// Take the squre root of a dimensioned quantity.
  /// @param v  Original dimensioned value.
  /// @return   Transformed value of different dimension.
  constexpr friend auto sqrt(dimval const &v) { return v.sqrt(); }

  /// Print to to output stream.
  friend std::ostream &operator<<(std::ostream &s, dimval const &v) {
    return s << v.v_ << v.d();
  }
};


/// Invert dimensioned value by dividing it into number.
/// This function is a friend of every dimval.
/// @tparam T  Type of numeric storage in dimval.
/// @tparam B  Dimension-type for dimval.
/// @param  d  Number as dividend.
/// @param  v  Dimensioned quantitity as divisor.
/// @return    Inverted value.
template <typename T, typename B>
constexpr auto operator/(double d, dimval<T, B> const &v) {
  auto const br = v.recip();
  return dimval<T, decltype(br)>(d / v.v_, br.d());
}


/// Invert dimensioned value by dividing it into number.
/// This function is a friend of every dimval.
/// @tparam T  Type of numeric storage in dimval.
/// @tparam B  Dimension-type for dimval.
/// @param  d  Number as dividend.
/// @param  v  Dimensioned quantitity as divisor.
/// @return    Inverted value.
template <typename T, typename B>
constexpr auto operator/(int d, dimval<T, B> const &v) {
  auto const br = v.recip();
  return dimval<T, decltype(br)>(d / v.v_, br.d());
}


/// Raise dimensioned value to rational power.
/// @tparam PN  Numerator of power.
/// @tparam PD  Denominator of power (by default, 1).
/// @tparam T   Type for numeric storage.
/// @tparam B   Type of base-class for dimension.
/// @param  v   Original dimensioned value.
/// @return     Transformed value of different dimension.
template <int64_t PN, int64_t PD = 1, typename T, typename B>
constexpr auto pow(dimval<T, B> const &v) {
  return v.template pow<PN, PD>();
}


/// Model of a dynamically dimensioned physical quantity.
/// @tparam T  Type of numeric value.
template <typename T> class basic_dyndim : public dimval<T, dyndim_base> {
  using dimval<T, dyndim_base>::v_; ///< Allow access to number.

public:
  /// Initialize from other dimensioned value.
  /// @tparam OT  Numeric type of other dimensioned value.
  /// @tparam OB  Dimension-base of other dimensioned value.
  /// @param  v   Reference to other dimensioned value.
  template <typename OT, typename OB>
  constexpr basic_dyndim(dimval<OT, OB> const &v)
      : dimval<T, dyndim_base>(v) {}

  /// Convert from number.
  /// @param v  Number.
  constexpr basic_dyndim(double v) : dimval<T, dyndim_base>(v, nul_dim) {}

  /// Convert from number.
  /// @param v  Number.
  constexpr basic_dyndim(int v) : dimval<T, dyndim_base>(v, nul_dim) {}

  // TBD: dyndim should have a constructor from std::string.
};


/// Model of a dynamically dimensioned physical quantity with double-precision
/// numeric value.
using dyndimd = basic_dyndim<double>;


/// Model of a dynamically dimensioned physical quantity with single-precision
/// numeric value.
using dyndimf = basic_dyndim<float>;


/// Model of a statically dimensioned physical quantity.
/// @tparam D  Encoding of dimension in `uint64_t`.
/// @tparam T  Type numeric value.
template <uint64_t D, typename T>
class basic_statdim : public dimval<T, statdim_base<D>> {
  /// Type of compatible statdim.
  /// @tparam OT  Type of numeric value.
  template <typename OT> using stat = dimval<OT, statdim_base<D>>;

protected:
  /// Inherit all of parent's constructors as protected so that descendants
  /// such as length, time, etc., may call ancestor's constructor from number
  /// and dimension.
  using dimval<T, statdim_base<D>>::dimval;

public:
  /// Initialize from compatible statdim.
  /// @tparam OT  Type of numeric value.
  /// @param  dv  Compatible statdim.
  template <typename OT>
  constexpr basic_statdim(stat<OT> dv) : stat<T>(dv.v_, dv.d()) {}

  /// Initialize from dyndim.
  /// @tparam OT  Numeric type of other dimensioned value.
  /// @param  v   Reference to other dimensioned value.
  template <typename OT>
  constexpr basic_statdim(dimval<OT, dyndim_base> const &v) : stat<T>(v) {}
};


/// Specialization of basic_statdim for dimensionless quantity.
template <typename T>
class basic_statdim<nul_dim, T> : public dimval<T, statdim_base<nul_dim>> {
  /// Type of compatible statdim.
  /// @tparam OT  Type of numeric value.
  template <typename OT> using stat = dimval<OT, statdim_base<nul_dim>>;

  using dimval<T, statdim_base<nul_dim>>::v_; ///< Allow access to number.

public:
  /// Initialize from compatible (dimensionless) statdim.
  /// @tparam OT  Type of numeric value.
  /// @param  dv  Compatible statdim.
  template <typename OT>
  constexpr basic_statdim(stat<OT> dv) : stat<T>(dv.v_, dv.d()) {}

  /// Initialize from dyndim.
  /// @tparam OT  Numeric type of other dimensioned value.
  /// @param  v   Reference to other dimensioned value.
  template <typename OT>
  constexpr basic_statdim(dimval<OT, dyndim_base> const &v) : stat<T>(v) {}

  /// Initialize from number.
  /// @param v  Number.
  constexpr basic_statdim(double v) : stat<T>(v, nul_dim) {}

  /// Initialize from number.
  /// @param v  Number.
  constexpr basic_statdim(int v) : stat<T>(v, nul_dim) {}

  constexpr operator T() const { return v_; } ///< Convert to number.
};


/// Model of a statically dimensioned physical quantity with double-precision
/// numeric value.
///
/// @tparam D  Encoding of dimension in `uint64_t`.
template <uint64_t D> using statdimd = basic_statdim<D, double>;


/// Model of a statically dimensioned physical quantity with single-precision
/// numeric value.
///
/// @tparam D  Encoding of dimension in `uint64_t`.
template <uint64_t D> using statdimf = basic_statdim<D, float>;


using dimensionlessd = statdimd<nul_dim>; ///< Double-precision dimensionless.
using timed          = statdimd<tim_dim>; ///< Double-precision time.
using lengthd        = statdimd<len_dim>; ///< Double-precision length.
using massd          = statdimd<mas_dim>; ///< Double-precision mass.
using charged        = statdimd<chg_dim>; ///< Double-precision charge.
using temperatured   = statdimd<tmp_dim>; ///< Double-precision temperature.


using dimensionlessf = statdimf<nul_dim>; ///< Single-precision dimensionless.
using timef          = statdimf<tim_dim>; ///< Single-precision time.
using lengthf        = statdimf<len_dim>; ///< Single-precision length.
using massf          = statdimf<mas_dim>; ///< Single-precision mass.
using chargef        = statdimf<chg_dim>; ///< Single-precision charge.
using temperaturef   = statdimf<tmp_dim>; ///< Single-precision temperature.


#ifdef VNIX_UNITS_DBL


/// Default precision for statdim.
/// @tparam D  Encoding of dimensions in uint64_t word.
template <uint64_t D> using statdim = statdimd<D>;

using dyndim        = dyndimd;        ///< Default precision for dyndim.
using dimensionless = dimensionlessd; ///< Default precision for dimensionless.
using time          = timed;          ///< Default precision for time.
using length        = lengthd;        ///< Default precision for length.
using mass          = massd;          ///< Default precision for mass.
using charge        = charged;        ///< Default precision for charge.
using temperature   = temperatured;   ///< Default precision for temperature.


#else // single precision


/// Default precision for statdim.
/// @tparam D  Encoding of dimensions in uint64_t word.
template <uint64_t D> using statdim = statdimf<D>;

using dyndim        = dyndimf;        ///< Default precision for dyndim.
using dimensionless = dimensionlessf; ///< Default precision for dimensionless.
using time          = timef;          ///< Default precision for time.
using length        = lengthf;        ///< Default precision for length.
using mass          = massf;          ///< Default precision for mass.
using charge        = chargef;        ///< Default precision for charge.
using temperature   = temperaturef;   ///< Default precision for temperature.


#endif // def VNIX_UNITS_DBL


/// Fundamental statdim-type for seconds.
struct seconds : public time {
  /// Initialize from number.
  /// @param v  Number of seconds.
  constexpr seconds(float v) : time(v, tim_dim) {}
};


/// Fundamental statdim-type for meters.
struct meters : public length {
  /// Initialize from number.
  /// @param v  Number of meters.
  constexpr meters(float v) : length(v, len_dim) {}
};


/// Fundamental statdim-type for kilograms.
struct kilograms : public mass {
  /// Initialize from number.
  /// @param v  Number of kilograms.
  constexpr kilograms(float v) : mass(v, mas_dim) {}
};


/// Fundamental statdim-type for coulombs.
struct coulombs : public charge {
  /// Initialize from number.
  /// @param v  Number of coulombs.
  constexpr coulombs(float v) : charge(v, chg_dim) {}
};


/// Fundamental statdim-type for kelvins.
struct kelvins : public temperature {
  /// Initialize from number.
  /// @param v  Number of kelvins.
  constexpr kelvins(float v) : temperature(v, tmp_dim) {}
};



} // namespace units
} // namespace vnix

#endif // ndef VNIX_UNITS_DIMVAL_HPP
