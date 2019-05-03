/// @file       vnix/units/dimval.hpp
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

/// Classes and functions supporting a model of physically dimensioned
/// quantities.
namespace units {


template <typename T, typename B> class dimval;
template <typename T> class basic_dyndim;
template <dim::word D, typename T> class basic_statdim;


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
  /// @tparam D   Encoding of dimension in dim::word.
  /// @tparam OT  Type of statdim's numeric value.
  template <dim::word D, typename OT> friend class basic_statdim;

protected:
  /// Initialize from numeric value and from dimension.
  /// @param v  Numeric value.
  /// @param d  Dimension.
  constexpr dimval(T v, dim const &d) : number<T>(v), B(d) {}

  /// Initialize dimension, but leave number undefined.
  /// @param d  Dimension.
  dimval(dim const &d) : B(d) {}

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
  constexpr dimval(T n) : number<T>(n), B(dim()) {}

  /// Convert to dimensionless number.
  constexpr T to_number() const {
    B::number();
    return v_;
  }

  /// Exponent for base at specified offset.
  /// @param off  Offset.
  constexpr dim::rat d(dim::off off) const { return B::d()[off]; }

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
    return dimval<decltype(T() + OT()), B>(v_ + v.v_, sdim.d());
  }

  /// Difference between two dimensioned values.
  /// @tparam OT  Numeric type of subtractor.
  /// @tparam OB  Base-dimension type of subtractor.
  /// @param  v   Subractor.
  /// @return     Difference.
  template <typename OT, typename OB>
  constexpr auto operator-(dimval<OT, OB> const &v) const {
    auto const ddim = B::diff(v); // Check for compatibility of units.
    return dimval<decltype(T() - OT()), B>(v_ - v.v_, ddim.d());
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
  /// @param  n   Scale-factor.
  /// @return     Scaled value.
  constexpr auto operator*(long double n) const {
    return dimval<decltype(T() * double()), B>(v_ * n, d());
  }

  /// Scale dimensioned value.
  /// @param  n   Scale-factor.
  /// @return     Scaled value.
  constexpr auto operator*(double n) const {
    return dimval<decltype(T() * double()), B>(v_ * n, d());
  }

  /// Scale dimensioned value.
  /// @param  n   Scale-factor.
  /// @return     Scaled value.
  constexpr auto operator*(float n) const {
    return dimval<decltype(T() * float()), B>(v_ * n, d());
  }

  /// Scale dimensioned value.
  /// @param  n   Scale-factor.
  /// @return     Scaled value.
  constexpr auto operator*(int n) const {
    return dimval<decltype(T() * int()), B>(v_ * n, d());
  }

  /// Scale dimensioned value.
  /// @param  n   Scale-factor.
  /// @param  v   Original value.
  /// @return     Scaled value.
  friend constexpr auto operator*(long double n, dimval const &v) {
    return v * n;
  }

  /// Scale dimensioned value.
  /// @param  n   Scale-factor.
  /// @param  v   Original value.
  /// @return     Scaled value.
  friend constexpr auto operator*(double n, dimval const &v) { return v * n; }

  /// Scale dimensioned value.
  /// @param  n   Scale-factor.
  /// @param  v   Original value.
  /// @return     Scaled value.
  friend constexpr auto operator*(float n, dimval const &v) { return v * n; }

  /// Scale dimensioned value.
  /// @param  n   Scale-factor.
  /// @param  v   Original value.
  /// @return     Scaled value.
  friend constexpr auto operator*(int n, dimval const &v) { return v * n; }

  using ldbl = long double;

  /// Scale dimensioned quantity by dividing by number.
  /// @param  n   Scale-divisor.
  constexpr auto operator/(long double n) const {
    return dimval<decltype(T() / ldbl()), B>(v_ / n, d());
  }

  /// Scale dimensioned quantity by dividing by number.
  /// @param  n   Scale-divisor.
  constexpr auto operator/(double n) const {
    return dimval<decltype(T() / double()), B>(v_ / n, d());
  }

  /// Scale dimensioned quantity by dividing by number.
  /// @param  n   Scale-divisor.
  constexpr auto operator/(float n) const {
    return dimval<decltype(T() / float()), B>(v_ / n, d());
  }

  /// Scale dimensioned quantity by dividing by number.
  /// @param  n   Scale-divisor.
  constexpr auto operator/(int n) const {
    return dimval<decltype(T() / int()), B>(v_ / n, d());
  }

  /// Invert dimensioned value.
  /// @return Reciprocal of dimval.
  constexpr dimval<T, typename B::recip_basedim> inverse() const {
    auto const br = this->recip();
    return dimval<T, typename B::recip_basedim>(1.0 / v_, br.d());
  }

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
  /// @param  v   Dimensionless scale-factor.
  /// @return     Scaled value.
  constexpr dimval &operator*=(long double v) {
    v_ *= v;
    return *this;
  }

  /// Modify present instance by multiplying in a dimensionless value.
  /// @param  v   Dimensionless scale-factor.
  /// @return     Scaled value.
  constexpr dimval &operator*=(double v) {
    v_ *= v;
    return *this;
  }

  /// Modify present instance by multiplying in a dimensionless value.
  /// @param  v   Dimensionless scale-factor.
  /// @return     Scaled value.
  constexpr dimval &operator*=(float v) {
    v_ *= v;
    return *this;
  }

  /// Modify present instance by multiplying in a dimensionless value.
  /// @param  v   Dimensionless scale-factor.
  /// @return     Scaled value.
  constexpr dimval &operator*=(int v) {
    v_ *= v;
    return *this;
  }

  /// Modify present instance by dividing it by a dimensionless value.
  /// @param  v   Dimensionless scale-divisor.
  /// @return     Scaled value.
  constexpr dimval &operator/=(long double v) {
    v_ /= v;
    return *this;
  }

  /// Modify present instance by dividing it by a dimensionless value.
  /// @param  v   Dimensionless scale-divisor.
  /// @return     Scaled value.
  constexpr dimval &operator/=(double v) {
    v_ /= v;
    return *this;
  }

  /// Modify present instance by dividing it by a dimensionless value.
  /// @param  v   Dimensionless scale-divisor.
  /// @return     Scaled value.
  constexpr dimval &operator/=(float v) {
    v_ /= v;
    return *this;
  }

  /// Modify present instance by dividing it by a dimensionless value.
  /// @param  v   Dimensionless scale-divisor.
  /// @return     Scaled value.
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
  constexpr auto pow(dim::rat p) const {
    auto const pdim = B::pow(p);
    using rt        = dimval<T, decltype(pdim)>;
    return rt(std::pow(v_, p.to_double()), pdim.d());
  }

  /// Take the squre root of a dimensioned quantity.
  constexpr auto sqrt() const {
    auto const rdim = B::sqrt();
    return dimval<T, decltype(rdim)>(std::sqrt(v_), rdim.d());
  }

  /// Print to to output stream.
  friend std::ostream &operator<<(std::ostream &s, dimval const &v) {
    return s << v.v_ << v.d();
  }
};


/// Invert dimensioned value by dividing it into number.
/// @tparam T   Type of numeric storage in dimval.
/// @tparam B   Type of base-class for dimension.
/// @param  d   Number as dividend.
/// @param  v   Dimensioned quantitity as divisor.
/// @return     Inverted value.
template <typename T, typename B>
constexpr auto operator/(long double d, dimval<T, B> const &v) {
  return d * v.inverse();
}


/// Invert dimensioned value by dividing it into number.
/// @tparam T   Type of numeric storage in dimval.
/// @tparam B   Type of base-class for dimension.
/// @param  d   Number as dividend.
/// @param  v   Dimensioned quantitity as divisor.
/// @return     Inverted value.
template <typename T, typename B>
constexpr auto operator/(double d, dimval<T, B> const &v) {
  return d * v.inverse();
}


/// Invert dimensioned value by dividing it into number.
/// @tparam T   Type of numeric storage in dimval.
/// @tparam B   Type of base-class for dimension.
/// @param  d   Number as dividend.
/// @param  v   Dimensioned quantitity as divisor.
/// @return     Inverted value.
template <typename T, typename B>
constexpr auto operator/(float d, dimval<T, B> const &v) {
  return d * v.inverse();
}


/// Invert dimensioned value by dividing it into number.
/// @tparam T   Type of numeric storage in dimval.
/// @tparam B   Type of base-class for dimension.
/// @param  d   Number as dividend.
/// @param  v   Dimensioned quantitity as divisor.
/// @return     Inverted value.
template <typename T, typename B>
constexpr auto operator/(int d, dimval<T, B> const &v) {
  return d * v.inverse();
}


/// Take the squre root of a dimensioned quantity.
/// @tparam T  Type for numeric storage.
/// @tparam B  Type of base-class for dimension.
/// @param  v  Original dimensioned value.
/// @return    Transformed value of different dimension.
template <typename T, typename B> constexpr auto sqrt(dimval<T, B> const &v) {
  return v.sqrt();
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


/// Raise dimensioned value to rational power.
/// @tparam T  Type for numeric storage.
/// @tparam B  Type of base-class for dimension.
/// @param  v  Original dimensioned value.
/// @param  p  Rational power.
/// @return    Transformed value of different dimension.
template <typename T, typename B>
constexpr auto pow(dimval<T, B> const &v, dim::rat p) {
  return v.pow(p);
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
  constexpr basic_dyndim(T v) : dimval<T, dyndim_base>(v, dim()) {}

  // TBD: dyndim should have a constructor from std::string.
};


/// Model of a statically dimensioned physical quantity.
/// @tparam D  Encoding of dimension in dim::word.
/// @tparam T  Type numeric value.
template <dim::word D, typename T>
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
  /// By default, initialize dimension, but leave number uninitialized.
  basic_statdim() : stat<T>(this->d()) {}

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


constexpr auto nul_code = nul_dim.encode();


/// Specialization of basic_statdim for dimensionless quantity.
template <typename T>
class basic_statdim<nul_code, T> : public dimval<T, statdim_base<nul_code>> {
  /// Type of compatible statdim.
  /// @tparam OT  Type of numeric value.
  template <typename OT> using stat = dimval<OT, statdim_base<nul_code>>;

  using dimval<T, statdim_base<nul_code>>::v_; ///< Allow access to number.

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
  constexpr basic_statdim(T v) : stat<T>(v, nul_dim) {}

  constexpr operator T() const { return v_; } ///< Convert to number.
};


} // namespace units
} // namespace vnix

#endif // ndef VNIX_UNITS_DIMVAL_HPP
