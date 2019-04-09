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

/// Model of a value with physical dimensions.
///
/// Associated with a dimensioned value is a set of exponents, one for each of
/// the five base dimensions (time, length, mass, charge, and temperature).
///
/// @tparam D  Encoding of dimensional exponents.
template <uint64_t D> class dimval {
protected:
  double v_; ///< Numeric value that multiplies units.

  /// Allow any kind of dimval to use any other kind's constructor from double.
  template <uint64_t OD> friend class dimval;

  /// Initialize numeric value and exponents of units.
  /// @param v  Numeric value that multiplies units.
  constexpr dimval(double v) : v_(v) {}

  /// Add exponents.
  constexpr static uint64_t sum_exp(uint64_t e1, uint64_t e2) {
    return dim(e1) + dim(e2);
  }

  /// Subtract exponents.
  constexpr static uint64_t diff_exp(uint64_t e1, uint64_t e2) {
    return dim(e1) - dim(e2);
  }

  /// Invert exponents.
  constexpr static uint64_t inv_exp(uint64_t e) { return null - dim(e); }

public:
  /// Exponent for each unit in dimensioned quantity.
  constexpr static dim d = dim(D);

  /// Multiply two dimensioned values.
  template <uint64_t OD>
  constexpr dimval<sum_exp(D, OD)> operator*(dimval<OD> v) const {
    return v_ * v.v_;
  }

  /// Divide two dimensioned values.
  template <uint64_t OD>
  constexpr dimval<diff_exp(D, OD)> operator/(dimval<OD> v) const {
    return v_ / v.v_;
  }

  /// Modify present instance by multiplying in a dimensionless value.
  constexpr dimval &operator*=(double v) {
    v_ *= v;
    return *this;
  }

  /// Modify present instance by dividing it by a dimensionless value.
  constexpr dimval &operator/=(double v) {
    v_ /= v;
    return *this;
  }

  /// Sum of two dimensioned values.
  constexpr dimval operator+(dimval v) const { return v_ + v.v_; }

  /// Difference between two dimensioned values.
  constexpr dimval operator-(dimval v) const { return v_ - v.v_; }

  /// Modify present instance by adding in a dimensioned value.
  constexpr dimval &operator+=(dimval v) {
    v_ += v;
    return *this;
  }

  /// Modify present instance by subtracting out a dimensioned value.
  constexpr dimval &operator-=(dimval v) {
    v_ -= v;
    return *this;
  }

  /// Scale dimensioned quantity.
  friend constexpr dimval operator*(int d, dimval v) { return d * v.v_; }

  /// Scale dimensioned quantity.
  friend constexpr dimval operator*(double d, dimval v) { return d * v.v_; }

  /// Scale dimensioned quantity.
  friend constexpr dimval operator*(dimval v, int d) { return d * v; }

  /// Scale dimensioned quantity.
  friend constexpr dimval operator*(dimval v, double d) { return d * v; }

  /// Invert dimensioned quantity by dividing it into number.
  friend constexpr dimval<inv_exp(D)> operator/(int d, dimval v) {
    return d / v.v_;
  }

  /// Invert dimensioned quantity by dividing it into number.
  friend constexpr dimval<inv_exp(D)> operator/(double d, dimval v) {
    return d / v.v_;
  }

  /// Scale dimensioned quantity by dividing by number.
  friend constexpr dimval operator/(dimval v, int d) { return v.v_ / d; }

  /// Scale dimensioned quantity by dividing by number.
  friend constexpr dimval operator/(dimval v, double d) { return v.v_ / d; }

  /// Half the current dimension for square-root.
  constexpr static uint64_t HD = dim(D) / rat(2);

  /// Take the squre root of a dimensioned quantity.
  constexpr dimval<HD> sqrt() { return std::sqrt(v_); }

  /// Rational multiple of current dimension for power.
  template <int64_t PN, int64_t PD>
  constexpr static uint64_t RD = dim(D) * rat(PN) / rat(PD);

  /// Raise dimensioned value to rational power.
  /// @tparam PN  Numerator of power.
  /// @tparam PD  Denominator of power (by default, 1).
  /// @return     Transformed value of different dimension.
  template <int64_t PN, int64_t PD = 1>
  constexpr dimval<RD<PN, PD>> pow() const {
    return std::pow(v_, PN * 1.0 / PD);
  }

  /// Print to to output stream.
  friend inline std::ostream &operator<<(std::ostream &s, dimval v) {
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
template <> class dimval<null> {
protected:
  double v_; ///< Numeric value that multiplies units.

public:
  /// Convert from double.
  constexpr dimval(double v) : v_(v) {}

  /// Convert to immutable double.
  constexpr operator double const &() const { return v_; }

  /// Convert to mutable double.
  constexpr operator double &() { return v_; }
};


struct seconds : public dimval<dim{1, 0, 0, 0, 0}> {
  constexpr seconds(double v) : dimval(v) {}
};


struct meters : public dimval<dim{0, 1, 0, 0, 0}> {
  constexpr meters(double v) : dimval(v) {}
};


struct kilograms : public dimval<dim{0, 0, 1, 0, 0}> {
  constexpr kilograms(double v) : dimval(v) {}
};


struct coulombs : public dimval<dim{0, 0, 0, 1, 0}> {
  constexpr coulombs(double v) : dimval(v) {}
};


struct kelvins : public dimval<dim{0, 0, 0, 0, 1}> {
  constexpr kelvins(double v) : dimval(v) {}
};


/// Take square-root of dimensioned value.
template <uint64_t D> constexpr auto sqrt(dimval<D> v) { return v.sqrt(); }


/// Raise dimensioned value to rational power.
/// @tparam PN  Numerator of power.
/// @tparam PD  Denominator of power (by default, 1).
/// @tparam D   Encoding of dimension for dimensioned value.
/// @param  v   Dimensioned value.
/// @return     Transformed value of different dimension.
template <int64_t PN, int64_t PD = 1, uint64_t D>
constexpr auto pow(dimval<D> v) {
  return v.template pow<PN, PD>();
}


} // namespace units
} // namespace vnix

#endif // ndef VNIX_UNITS_DIMVAL_HPP
