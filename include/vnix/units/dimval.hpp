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


constexpr uint64_t encode(dim d) {
  return (uint64_t(rat::encode(d[TIM])) << (TIM * 8)) |
         (uint64_t(rat::encode(d[LEN])) << (LEN * 8)) |
         (uint64_t(rat::encode(d[MAS])) << (MAS * 8)) |
         (uint64_t(rat::encode(d[CHG])) << (CHG * 8)) |
         (uint64_t(rat::encode(d[TMP])) << (TMP * 8));
}


constexpr dim decode(uint64_t u) {
  return {{rat::decode((u >> (TIM * 8)) & 0xFF),
           rat::decode((u >> (LEN * 8)) & 0xFF),
           rat::decode((u >> (MAS * 8)) & 0xFF),
           rat::decode((u >> (CHG * 8)) & 0xFF),
           rat::decode((u >> (TMP * 8)) & 0xFF)}};
}


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
  /// @tparam OD  Encoding of other statdim's dimension.
  template <uint64_t OD> friend class statdim;

  using dimval_base::dimval_base; /// Inherit constructor.

  /// Add exponents.
  constexpr static uint64_t sum_exp(uint64_t e1, uint64_t e2) {
    return encode(decode(e1) + decode(e2));
  }

  /// Subtract exponents.
  constexpr static uint64_t diff_exp(uint64_t e1, uint64_t e2) {
    return encode(decode(e1) - decode(e2));
  }

  /// Invert exponents.
  constexpr static uint64_t inv_exp(uint64_t e) {
    return encode(nul_dim - decode(e));
  }

public:
  /// Exponent for each unit in dimensioned quantity.
  constexpr static dim d = decode(D);

  /// Multiply two dimensioned values.
  template <uint64_t OD> constexpr auto operator*(statdim<OD> v) const {
    return statdim<sum_exp(D, OD)>(v_ * v.v_);
  }

  /// Divide two dimensioned values.
  template <uint64_t OD> constexpr auto operator/(statdim<OD> v) const {
    return statdim<diff_exp(D, OD)>(v_ / v.v_);
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
  constexpr statdim operator+(statdim v) const { return v_ + v.v_; }

  /// Difference between two dimensioned values.
  constexpr statdim operator-(statdim v) const { return v_ - v.v_; }

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
  friend constexpr statdim operator*(int d, statdim v) { return d * v.v_; }

  /// Scale dimensioned quantity.
  friend constexpr statdim operator*(double d, statdim v) { return d * v.v_; }

  /// Scale dimensioned quantity.
  friend constexpr statdim operator*(statdim v, int d) { return d * v; }

  /// Scale dimensioned quantity.
  friend constexpr statdim operator*(statdim v, double d) { return d * v; }

  /// Invert dimensioned quantity by dividing it into number.
  friend constexpr statdim<inv_exp(D)> operator/(int d, statdim v) {
    return d / v.v_;
  }

  /// Invert dimensioned quantity by dividing it into number.
  friend constexpr statdim<inv_exp(D)> operator/(double d, statdim v) {
    return d / v.v_;
  }

  /// Scale dimensioned quantity by dividing by number.
  friend constexpr statdim operator/(statdim v, int d) { return v.v_ / d; }

  /// Scale dimensioned quantity by dividing by number.
  friend constexpr statdim operator/(statdim v, double d) { return v.v_ / d; }

  /// Half the current dimension for square-root.
  constexpr static uint64_t HD = encode(decode(D) / rat(2));

  /// Take the squre root of a dimensioned quantity.
  constexpr auto sqrt() { return statdim<HD>(std::sqrt(v_)); }

  /// Rational multiple of current dimension for power.
  template <int64_t PN, int64_t PD>
  constexpr static uint64_t RD = encode(decode(D) * rat(PN) / rat(PD));

  /// Raise dimensioned value to rational power.
  /// @tparam PN  Numerator of power.
  /// @tparam PD  Denominator of power (by default, 1).
  /// @return     Transformed value of different dimension.
  template <int64_t PN, int64_t PD = 1> constexpr auto pow() const {
    return statdim<RD<PN, PD>>(std::pow(v_, PN * 1.0 / PD));
  }

  /// Print to to output stream.
  friend inline std::ostream &operator<<(std::ostream &s, statdim v) {
    s << v.v_;
    impl::print_unit(s, "m", v.d[LEN]);
    impl::print_unit(s, "kg", v.d[MAS]);
    impl::print_unit(s, "s", v.d[TIM]);
    impl::print_unit(s, "C", v.d[CHG]);
    impl::print_unit(s, "K", v.d[TMP]);
    return s;
  }
};


/// Specialization for dimensionless value.
template <> struct statdim<encode(nul_dim)> : public dimval_base {
  using dimval_base::dimval_base; /// Inherit constructor publicly.

  /// Convert to immutable double.
  constexpr operator double const &() const { return v_; }

  /// Convert to mutable double.
  constexpr operator double &() { return v_; }
};


struct seconds : public statdim<encode(tim_dim)> {
  constexpr seconds(double v) : statdim(v) {}
};


struct meters : public statdim<encode(len_dim)> {
  constexpr meters(double v) : statdim(v) {}
};


struct kilograms : public statdim<encode(mas_dim)> {
  constexpr kilograms(double v) : statdim(v) {}
};


struct coulombs : public statdim<encode(chg_dim)> {
  constexpr coulombs(double v) : statdim(v) {}
};


struct kelvins : public statdim<encode(tmp_dim)> {
  constexpr kelvins(double v) : statdim(v) {}
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
