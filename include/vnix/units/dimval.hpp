/// @file       units/dimval.hpp
/// @brief      Definition of vnix::units::dimval.
/// @copyright  2019 Thomas E. Vaughan
/// @license    GPL3.

#ifndef VNIX_UNITS_DIMVAL_HPP
#define VNIX_UNITS_DIMVAL_HPP

#include <cmath> // for sqrt
#include <vnix/units/dim.hpp>
#include <vnix/units/impl/print-unit.hpp>

namespace vnix {
namespace units {


/// Storage for numeric value associated with a physical dimension.
class dimval_base {
protected:
  double v_; ///< Numeric value that multiplies units.

  /// Initialize numeric value and exponents of units.
  /// @param v  Numeric value that multiplies units.
  constexpr dimval_base(double v) : v_(v) {}

  /// Allow any kind of statdim to use any other kind's constructor from
  /// double.
  /// @tparam OD  Encoding of other statdim's dimension.
  template <uint64_t OD> friend class statdim;
};


template <uint64_t D> class statdim;


template <typename DV1, typename DV2> struct binary_op {
  constexpr static dim prod_dim = DV1::d() + DV2::d();
  constexpr static dim quot_dim = DV1::d() - DV2::d();
  using prod                    = statdim<uint64_t(prod_dim)>;
  using quot                    = statdim<uint64_t(quot_dim)>;
};


template <typename DV> struct unary_op {
  constexpr static dim recip_dim = nul_dim - DV::d();
  using recip                    = statdim<uint64_t(recip_dim)>;
};


/// Model of a value with statically determined physical dimensions.
///
/// Associated with a dimensioned value is a set of exponents, one for each of
/// the five base dimensions (time, length, mass, charge, and temperature).
///
/// @tparam D  Encoding of dimensional exponents.
template <uint64_t D> class statdim : public dimval_base {
protected:
  /// Allow any kind of statdim to use any other kind's constructor from
  /// double.
  ///
  /// @tparam OD  Encoding of other statdim's dimension.
  template <uint64_t OD> friend class statdim;

  /// Initialize from numeric value.
  ///
  /// The second argument of the constructor is unused by statdim because the
  /// dimension is encoded in the uint64_t template-value-parameter.
  ///
  /// @param v  Numeric value.
  constexpr statdim(double v, dim) : dimval_base(v) {}

public:
  /// Exponent for each unit in dimensioned quantity.
  constexpr static dim d() { return dim(D); }

  /// Exponent for base at specified offset.
  /// @param off  Offset.
  constexpr static rat d(base_off off) { return dim(D)[off]; }

  /// Multiply two dimensioned values.
  template <typename DV> constexpr auto operator*(DV v) const {
    using op = binary_op<statdim, DV>;
    return typename op::prod(v_ * v.v_, op::prod_dim);
  }

  /// Divide two dimensioned values.
  template <typename DV> constexpr auto operator/(DV v) const {
    using op = binary_op<statdim, DV>;
    return typename op::quot(v_ / v.v_, op::quot_dim);
  }

  /// Modify present instance by multiplying in a dimensionless value.
  constexpr statdim &operator*=(double v) {
    v_ *= v;
    return *this;
  }

  /// Modify present instance by dividing it by a dimensionless value.
  constexpr statdim &operator/=(double v) {
    v_ /= v;
    return *this;
  }

  /// Sum of two dimensioned values.
  constexpr auto operator+(statdim v) const { return statdim(v_ + v.v_, d()); }

  /// Difference between two dimensioned values.
  constexpr auto operator-(statdim v) const { return statdim(v_ - v.v_, d()); }

  /// Modify present instance by adding in a dimensioned value.
  constexpr statdim &operator+=(statdim v) {
    v_ += v;
    return *this;
  }

  /// Modify present instance by subtracting out a dimensioned value.
  constexpr statdim &operator-=(statdim v) {
    v_ -= v;
    return *this;
  }

  /// Scale dimensioned quantity.
  friend constexpr auto operator*(int n, statdim v) {
    return statdim(n * v.v_, d());
  }

  /// Scale dimensioned quantity.
  friend constexpr auto operator*(double n, statdim v) {
    return statdim(n * v.v_, d());
  }

  /// Scale dimensioned quantity.
  friend constexpr auto operator*(statdim v, int n) { return n * v; }

  /// Scale dimensioned quantity.
  friend constexpr auto operator*(statdim v, double n) { return n * v; }

  /// Invert dimensioned quantity by dividing it into number.
  friend constexpr auto operator/(int d, statdim v) {
    using op = unary_op<statdim>;
    return typename op::recip(d / v.v_, op::recip_dim);
  }

  /// Invert dimensioned quantity by dividing it into number.
  friend constexpr auto operator/(double d, statdim v) {
    using op = unary_op<statdim>;
    return typename op::recip(d / v.v_, op::recip_dim);
  }

  /// Scale dimensioned quantity by dividing by number.
  friend constexpr auto operator/(statdim v, int n) {
    return statdim(v.v_ / n, d());
  }

  /// Scale dimensioned quantity by dividing by number.
  friend constexpr auto operator/(statdim v, double n) {
    return statdim(v.v_ / n, d());
  }

  /// Take the squre root of a dimensioned quantity.
  constexpr auto sqrt() {
    constexpr dim HD = d() / rat(2); // Half the current dimension.
    return statdim<uint64_t(HD)>(std::sqrt(v_), HD);
  }

  /// Raise dimensioned value to rational power.
  /// @tparam PN  Numerator of power.
  /// @tparam PD  Denominator of power (by default, 1).
  /// @return     Transformed value of different dimension.
  template <int64_t PN, int64_t PD = 1> constexpr auto pow() const {
    /// Rational multiple of current dim.
    constexpr dim RD = dim(D) * rat(PN) / rat(PD);
    return statdim<uint64_t(RD)>(std::pow(v_, PN * 1.0 / PD), RD);
  }

  /// Print to to output stream.
  friend inline std::ostream &operator<<(std::ostream &s, statdim v) {
    s << v.v_;
    impl::print_unit(s, "m", v.d(LEN));
    impl::print_unit(s, "kg", v.d(MAS));
    impl::print_unit(s, "s", v.d(TIM));
    impl::print_unit(s, "C", v.d(CHG));
    impl::print_unit(s, "K", v.d(TMP));
    return s;
  }
};


/// Specialization for dimensionless value.
template <> struct statdim<nul_dim> : public dimval_base {
  /// Allow any kind of statdim to use null statdim's constructor from double.
  /// @tparam OD  Encoding of other statdim's dimension.
  template <uint64_t OD> friend class statdim;

  using dimval_base::dimval_base; /// Inherit constructor publicly.

  /// Convert to immutable double.
  constexpr operator double const &() const { return v_; }

  /// Convert to mutable double.
  constexpr operator double &() { return v_; }

  constexpr static dim d() { return nul_dim; }

protected:
  statdim(double v, dim) : dimval_base(v) {}
};


struct seconds : public statdim<tim_dim> {
  constexpr seconds(double v) : statdim(v, tim_dim) {}
};


struct meters : public statdim<len_dim> {
  constexpr meters(double v) : statdim(v, len_dim) {}
};


struct kilograms : public statdim<mas_dim> {
  constexpr kilograms(double v) : statdim(v, mas_dim) {}
};


struct coulombs : public statdim<chg_dim> {
  constexpr coulombs(double v) : statdim(v, chg_dim) {}
};


struct kelvins : public statdim<tmp_dim> {
  constexpr kelvins(double v) : statdim(v, tmp_dim) {}
};


/// Take square-root of dimensioned value.
template <uint64_t D> constexpr auto sqrt(statdim<D> v) { return v.sqrt(); }


/// Raise dimensioned value to rational power.
/// @tparam PN  Numerator of power.
/// @tparam PD  Denominator of power (by default, 1).
/// @tparam D   Encoding of dimension for dimensioned value.
/// @param  v   Dimensioned value.
/// @return     Transformed value of different dimension.
template <int64_t PN, int64_t PD = 1, uint64_t D>
constexpr auto pow(statdim<D> v) {
  return v.template pow<PN, PD>();
}


} // namespace units
} // namespace vnix

#endif // ndef VNIX_UNITS_DIMVAL_HPP
