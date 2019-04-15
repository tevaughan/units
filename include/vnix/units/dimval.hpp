/// @file       units/dimval.hpp
/// @brief      Definition of vnix::units::dimval.
/// @copyright  2019  Thomas E. Vaughan
/// @license    GPL Version 3 or later.

#ifndef VNIX_UNITS_DIMVAL_HPP
#define VNIX_UNITS_DIMVAL_HPP

#include <cmath>                       // for sqrt
#include <vnix/units/dim.hpp>          // for dim
#include <vnix/units/dyndim-base.hpp>  // for dyndim_base
#include <vnix/units/number.hpp>       // for number
#include <vnix/units/print-unit.hpp>   // for print_unit
#include <vnix/units/statdim-base.hpp> // for statdim_base

namespace vnix {
namespace units {


template <typename T> struct dyndim;
template <uint64_t D, typename T> class statdim;


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
  template <typename OT> friend struct dyndim;

  /// Allow access to every kind of statdim.
  /// @tparam D   Encoding of dimension in `uint64_t`.
  /// @tparam OT  Type of statdim's numeric value.
  template <uint64_t D, typename OT> friend class statdim;

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
  constexpr dimval operator*(T n) const { return {v_ * n, d()}; }

  /// Scale dimensioned value.
  /// @param n  Scale-factor.
  /// @param v  Original value.
  /// @return   Scaled value.
  friend constexpr dimval operator*(T n, dimval const &v) { return v * n; }

  /// Invert dimensioned value by dividing it into number.
  /// @param d  Number as dividend.
  /// @param v  Dimensioned quantitity as divisor.
  /// @return   Inverted value.
  friend constexpr auto operator/(T d, dimval const &v) {
    auto const br = v.recip();
    return dimval<T, decltype(br)>(d / v.v_, br.d());
  }

  /// Scale dimensioned quantity by dividing by number.
  constexpr dimval operator/(T n) const { return {v_ / n, d()}; }

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
  constexpr dimval &operator*=(T v) {
    v_ *= v;
    return *this;
  }

  /// Modify present instance by dividing it by a dimensionless value.
  /// @param v  Dimensionless scale-divisor.
  /// @return   Scaled value.
  constexpr dimval &operator/=(T v) {
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
    s << v.v_;
    print_unit(s, "m", v.d(LEN));
    print_unit(s, "kg", v.d(MAS));
    print_unit(s, "s", v.d(TIM));
    print_unit(s, "C", v.d(CHG));
    print_unit(s, "K", v.d(TMP));
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


/// Model of a dynamically dimensioned physical quantity.
/// @tparam T  Type of numeric value.
template <typename T> struct dyndim : public dimval<T, dyndim_base> {
  /// Allow dyndim to be initialized from any other dimval whatsoever.
  /// @tparam OT  Other dimval's numeric type.
  /// @tparam OB  Other dimval's base-dimension type.
  /// @param  dv  Other dimval.
  template <typename OT, typename OB>
  dyndim(dimval<OT, OB> const &dv) : dimval<T, dyndim_base>(dv.v_, dv.d()) {}

  // TBD: dyndim should have a constructor from std::string.
};


/// Model of a dynamically dimensioned physical quantity with double-precision
/// numeric value.
using dyndimd = dyndim<double>;


/// Model of a dynamically dimensioned physical quantity with single-precision
/// numeric value.
using dyndimf = dyndim<float>;


/// Model of a statically dimensioned physical quantity.
/// @tparam D  Encoding of dimension in `uint64_t`.
/// @tparam T  Type numeric value.
template <uint64_t D, typename T>
class statdim : public dimval<T, statdim_base<D>> {
  /// Type of compatible statdim.
  /// @tparam OT  Type of numeric value.
  template <typename OT> using stat = dimval<OT, statdim_base<D>>;

protected:
  using dimval<T, statdim_base<D>>::dimval; ///< Inherit constructor.

public:
  /// Initialize from compatible statdim.
  /// @tparam OT  Type of numeric value.
  /// @param  dv  Compatible statdim.
  template <typename OT> statdim(stat<OT> dv) : stat<T>(dv.v_, dv.d()) {}

  /// Attempt initialization from dyndim.  statdim_base's constructor will
  /// throw exception on incompatible dimension.
  ///
  /// @tparam OT  Type of numeric value.
  /// @param  dv  Instance of dyndim.
  template <typename OT>
  statdim(dyndim<OT> const &dv) : stat<T>(dv.v_, dv.d()) {}
};


/// Model of a statically dimensioned physical quantity with double-precision
/// numeric value.
///
/// @tparam D  Encoding of dimension in `uint64_t`.
template <uint64_t D> using statdimd = statdim<D, double>;


/// Model of a statically dimensioned physical quantity with single-precision
/// numeric value.
///
/// @tparam D  Encoding of dimension in `uint64_t`.
template <uint64_t D> using statdimf = statdim<D, float>;


using timed        = statdim<tim_dim, double>;
using lengthd      = statdim<len_dim, double>;
using massd        = statdim<mas_dim, double>;
using charged      = statdim<chg_dim, double>;
using temperatured = statdim<tmp_dim, double>;


using timef        = statdim<tim_dim, float>;
using lengthf      = statdim<len_dim, float>;
using massf        = statdim<mas_dim, float>;
using chargef      = statdim<chg_dim, float>;
using temperaturef = statdim<tmp_dim, float>;


#ifdef VNIX_UNITS_DBL

struct seconds : public timed {
  constexpr seconds(double v) : timed(v, tim_dim) {}
};

struct meters : public lengthd {
  constexpr meters(double v) : lengthd(v, len_dim) {}
};

struct kilograms : public massd {
  constexpr kilograms(double v) : massd(v, mas_dim) {}
};

struct coulombs : public charged {
  constexpr coulombs(double v) : charged(v, chg_dim) {}
};

struct kelvins : public temperatured {
  constexpr kelvins(double v) : temperatured(v, tmp_dim) {}
};

#else

struct seconds : public timef {
  constexpr seconds(float v) : timef(v, tim_dim) {}
};

struct meters : public lengthf {
  constexpr meters(float v) : lengthf(v, len_dim) {}
};

struct kilograms : public massf {
  constexpr kilograms(float v) : massf(v, mas_dim) {}
};

struct coulombs : public chargef {
  constexpr coulombs(float v) : chargef(v, chg_dim) {}
};

struct kelvins : public temperaturef {
  constexpr kelvins(float v) : temperaturef(v, tmp_dim) {}
};

#endif // def VNIX_UNITS_DBL


} // namespace units
} // namespace vnix

#endif // ndef VNIX_UNITS_DIMVAL_HPP
