/// @file       units/dimval.hpp
/// @brief      Definition of vnix::units::dimval.
/// @copyright  2019 Thomas E. Vaughan
/// @license    GPL3.

#ifndef VNIX_UNITS_DIMVAL_HPP
#define VNIX_UNITS_DIMVAL_HPP

#include <cmath>                          // for sqrt
#include <vnix/units/dim.hpp>             // for dim
#include <vnix/units/impl/print-unit.hpp> // for print_unit
#include <vnix/units/number.hpp>          // for number
#include <vnix/units/statdim-base.hpp>    // for statdim_base

namespace vnix {
namespace units {


/// Model of a physically dimensioned quantity.
/// @tparam T  Type of storage (e.g., float or double) for numerical quantity.
/// @tparam B  Base-class (statdim_base or dyndim_base) for dimension.
template <typename T, typename B> class dimval : public number<T>, public B {
  /// Allow every kind of dimval to access this dimval's contructor.
  /// @tparam OT  Type of other dimval's numeric value.
  /// @tparam OB  Type of other dimval's base-class for dimension.
  template <typename OT, typename OB> friend class dimval;

protected:
  /// Initialize from numeric value and from dimension.
  /// @param v  Numeric value.
  /// @param d  Dimension.
  constexpr dimval(T v, dim const &d) : number<T>(v), B(d) {}

  using number<T>::v_; ///< Allow access to numeric value.

public:
  using B::d; ///< Allow access to dimension.

  /// Exponent for base at specified offset.
  /// @param off  Offset.
  constexpr rat d(base_off off) const { return B::d()[off]; }

  /// Sum of two dimensioned values.
  /// @tparam OT  Numeric type of addend.
  /// @tparam OB  Base-dimension type of addend.
  /// @param  v   Addend.
  /// @return     Sum.
  template <typename OT, typename OB>
  constexpr auto operator+(dimval<OT, OB> const &v) const {
    auto const sdim = B::sum(v); // Check for compatibility of units.
    return dimval(v_ + v.v_, sdim);
  }

  /// Difference between two dimensioned values.
  /// @tparam OT  Numeric type of subtractor.
  /// @tparam OB  Base-dimension type of subtractor.
  /// @param  v   Subractor.
  /// @return     Difference.
  template <typename OT, typename OB>
  constexpr auto operator-(dimval<OT, OB> const &v) const {
    auto const ddim = B::diff(v); // Check for compatibility of units.
    return dimval(v_ - v.v_, ddim);
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
  /// @param v  Original value.
  /// @return   Scaled value.
  friend constexpr dimval operator*(double n, dimval const &v) {
    return v * n;
  }

  /// Invert dimensioned value by dividing it into number.
  /// @param d  Number as dividend.
  /// @param v  Dimensioned quantitity as divisor.
  /// @return   Inverted value.
  friend constexpr auto operator/(double d, dimval const &v) {
    auto const br = v.recip();
    return dimval<T, decltype(br)>(d / v.v_, br.d());
  }

  /// Scale dimensioned quantity by dividing by number.
  constexpr dimval operator/(double n) const { return {v_ / n, d()}; }

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

  /// Modify present instance by dividing it by a dimensionless value.
  /// @param v  Dimensionless scale-divisor.
  /// @return   Scaled value.
  constexpr dimval &operator/=(double v) {
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
    return rt(std::pow(v_, p.n() * 1.0 / p.d()), pdim.d());
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
    s << v.v_;
    impl::print_unit(s, "m", v.d(LEN));
    impl::print_unit(s, "kg", v.d(MAS));
    impl::print_unit(s, "s", v.d(TIM));
    impl::print_unit(s, "C", v.d(CHG));
    impl::print_unit(s, "K", v.d(TMP));
    return s;
  }
};


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


using time        = dimval<double, statdim_base<tim_dim>>;
using length      = dimval<double, statdim_base<len_dim>>;
using mass        = dimval<double, statdim_base<mas_dim>>;
using charge      = dimval<double, statdim_base<chg_dim>>;
using temperature = dimval<double, statdim_base<tmp_dim>>;


struct seconds : public time {
  constexpr seconds(double v) : time(v, tim_dim) {}
};


struct meters : public length {
  constexpr meters(double v) : length(v, len_dim) {}
};


struct kilograms : public mass {
  constexpr kilograms(double v) : mass(v, mas_dim) {}
};


struct coulombs : public charge {
  constexpr coulombs(double v) : charge(v, chg_dim) {}
};


struct kelvins : public temperature {
  constexpr kelvins(double v) : temperature(v, tmp_dim) {}
};


} // namespace units
} // namespace vnix

#endif // ndef VNIX_UNITS_DIMVAL_HPP
