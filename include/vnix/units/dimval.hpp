/// @file       units/dimval.hpp
/// @brief      Definition of vnix::units::dimval.
/// @copyright  2019 Thomas E. Vaughan
/// @license    GPL3.

#ifndef VNIX_UNITS_DIMVAL_HPP
#define VNIX_UNITS_DIMVAL_HPP

#include <vnix/units/dimension.hpp>
#include <vnix/units/impl/print-unit.hpp>

namespace vnix {
namespace units {


class dimval {
protected:
  double    v_; ///< Numeric value that multiplies units.
  dimension d_; ///< Exponent for each unit in dimensioned quantity.

public:
  /// Exponent for each unit in dimensioned quantity.
  constexpr dimension d() const { return d_; }
  /// Initialize numeric value and exponents of units.
  /// @param v  Numeric value that multiplies units.
  /// @param d  Exponent for each unit in dimensioned quantity.
  constexpr dimval(double v, dimension d) : v_(v), d_(d) {}
  /// Convert unitless quantity to double. Throw exception when instance has
  /// any nonzero exponents in the dimension.
  constexpr operator double() const {
    if (d_ == null) {
      return v_;
    }
    throw "attempted conversion of dimensioned value to undimensioned value";
  }
  /// Multiply two dimensioned values.
  constexpr dimval operator*(dimval v) const {
    return dimval(v_ * v.v_, d_ + v.d_);
  }
  /// Divide two dimensioned values.
  constexpr dimval operator/(dimval v) const {
    return dimval(v_ / v.v_, d_ - v.d_);
  }
  /// Modify present instance by multiplying in a dimensioned value.
  constexpr dimval &operator*=(dimval v) {
    *this = *this * v;
    return *this;
  }
  /// Modify present instance by dividing it by a dimensioned value.
  constexpr dimval &operator/=(dimval v) {
    *this = *this / v;
    return *this;
  }
  /// Sum of two dimensioned values. Throw exception when dimensions mismatch.
  constexpr dimval operator+(dimval v) const {
    if (d_ == v.d_) {
      return dimval(v_ + v.v_, d_);
    }
    throw "attempted sum of values with mismatched dimensions";
  }
  /// Difference of two dimensioned values. Throw exception when dimensions
  /// mismatch.
  constexpr dimval operator-(dimval v) const {
    if (d_ == v.d_) {
      return dimval(v_ - v.v_, d_);
    }
    throw "attempted different of values with mismatched dimensions";
  }
  /// Modify present instance by adding in a dimensioned value.  Throw
  /// exception when dimensions mismatch.
  constexpr dimval &operator+=(dimval v) {
    *this = *this + v;
    return *this;
  }
  /// Modify present instance by subtracting out a dimensioned value.  Throw
  /// exception when dimensions mismatch.
  constexpr dimval &operator-=(dimval v) {
    *this = *this - v;
    return *this;
  }
  /// Sum of number and dimensionless quantity.  Throw exception if dimensions
  /// be not null.
  friend constexpr double operator+(int d, dimval v) {
    if (v.d_ == null) {
      return d + v.v_;
    }
    throw "attempted sum of double and dimensioned quantity";
  }
  /// Sum of number and dimensionless quantity.  Throw exception if dimensions
  /// be not null.
  friend constexpr double operator+(double d, dimval v) {
    if (v.d_ == null) {
      return d + v.v_;
    }
    throw "attempted sum of double and dimensioned quantity";
  }
  /// Sum of number and dimensionless quantity.  Throw exception if dimensions
  /// be not null.
  friend constexpr double operator+(dimval v, int d) { return d + v; }
  /// Sum of number and dimensionless quantity.  Throw exception if dimensions
  /// be not null.
  friend constexpr double operator+(dimval v, double d) { return d + v; }
  /// Difference between number and dimensionless quantity.  Throw exception if
  /// dimensions be not null.
  friend constexpr double operator-(int d, dimval v) {
    if (v.d_ == null) {
      return d - v.v_;
    }
    throw "attempted sum of double and dimensioned quantity";
  }
  /// Difference between number and dimensionless quantity.  Throw exception if
  /// dimensions be not null.
  friend constexpr double operator-(double d, dimval v) {
    if (v.d_ == null) {
      return d - v.v_;
    }
    throw "attempted sum of double and dimensioned quantity";
  }
  /// Difference between number and dimensionless quantity.  Throw exception if
  /// dimensions be not null.
  friend constexpr double operator-(dimval v, int d) { return d - v; }
  /// Difference between number and dimensionless quantity.  Throw exception if
  /// dimensions be not null.
  friend constexpr double operator-(dimval v, double d) { return d - v; }
  /// Modify number by adding in dimensionless quantity.  Throw exception if
  /// dimensions be not null.
  friend constexpr double &operator+=(double &d, dimval v) {
    if (v.d_ == null) {
      return d += v.v_;
    }
    throw "attempted sum of double and dimensioned quantity";
  }
  /// Modify dimensionless quantity by adding in number.  Throw exception if
  /// dimensions be not null.
  friend constexpr dimval &operator+=(dimval &v, int d) {
    if (v.d_ == null) {
      v.v_ += d;
      return v;
    }
    throw "attempted sum of double and dimensioned quantity";
  }
  /// Modify dimensionless quantity by adding in number.  Throw exception if
  /// dimensions be not null.
  friend constexpr dimval &operator+=(dimval &v, double d) {
    if (v.d_ == null) {
      v.v_ += d;
      return v;
    }
    throw "attempted sum of double and dimensioned quantity";
  }
  /// Modify number by subtracting out dimensionless quantity.  Throw exception
  /// if dimensions be not null.
  friend constexpr double &operator-=(double &d, dimval v) {
    if (v.d_ == null) {
      return d -= v.v_;
    }
    throw "attempted sum of double and dimensioned quantity";
  }
  /// Modify dimensionless quantity by subtracting out number.  Throw exception
  /// if dimensions be not null.
  friend constexpr dimval &operator-=(dimval &v, int d) {
    if (v.d_ == null) {
      v.v_ -= d;
      return v;
    }
    throw "attempted sum of double and dimensioned quantity";
  }
  /// Modify dimensionless quantity by subtracting out number.  Throw exception
  /// if dimensions be not null.
  friend constexpr dimval &operator-=(dimval &v, double d) {
    if (v.d_ == null) {
      v.v_ -= d;
      return v;
    }
    throw "attempted sum of double and dimensioned quantity";
  }
  /// Scale dimensioned quantity.
  friend constexpr dimval operator*(int d, dimval v) {
    return dimval(d * v.v_, v.d_);
  }
  /// Scale dimensioned quantity.
  friend constexpr dimval operator*(double d, dimval v) {
    return dimval(d * v.v_, v.d_);
  }
  /// Scale dimensioned quantity.
  friend constexpr dimval operator*(dimval v, int d) { return d * v; }
  /// Scale dimensioned quantity.
  friend constexpr dimval operator*(dimval v, double d) { return d * v; }
  /// Invert dimensioned quantity by dividing it into number.
  friend constexpr dimval operator/(int d, dimval v) {
    return dimval(d / v.v_, -v.d_);
  }
  /// Invert dimensioned quantity by dividing it into number.
  friend constexpr dimval operator/(double d, dimval v) {
    return dimval(d / v.v_, -v.d_);
  }
  /// Scale dimensioned quantity by dividing by number.
  friend constexpr dimval operator/(dimval v, int d) {
    return dimval(v.v_ / d, +v.d_);
  }
  /// Scale dimensioned quantity by dividing by number.
  friend constexpr dimval operator/(dimval v, double d) {
    return dimval(v.v_ / d, +v.d_);
  }
  /// Modify number by multiplying in dimensionless quantity.  Throw exception
  /// if dimensions be not null.
  friend constexpr double &operator*=(double &d, dimval v) {
    if (v.d_ == null) {
      return d *= v.v_;
    }
    throw "attempted modifcation of number by multiplying dimensioned value";
  }
  /// Modify dimensioned value by scaling by dimensioned quantity.
  friend constexpr dimval &operator*=(dimval &v, int d) {
    v.v_ *= d;
    return v;
  }
  /// Modify dimensioned value by scaling by dimensioned quantity.
  friend constexpr dimval &operator*=(dimval &v, double d) {
    v.v_ *= d;
    return v;
  }
  /// Modify number by dividing by dimensionless quantity.  Throw exception if
  /// dimensions be not null.
  friend constexpr double &operator/=(double &d, dimval v) {
    if (v.d_ == null) {
      return d /= v.v_;
    }
    throw "attempted modification of number by dividing by dimensioned value";
  }
  /// Modify dimensioned value by dividing by number.
  friend constexpr dimval &operator/=(dimval &v, int d) {
    v.v_ /= d;
    return v;
  }
  /// Modify dimensioned value by dividing by number.
  friend constexpr dimval &operator/=(dimval &v, double d) {
    v.v_ /= d;
    return v;
  }
  /// Print to to output stream.
  friend inline std::ostream &operator<<(std::ostream &s, dimval v) {
    s << v.v_;
    impl::print_unit(s, "m", v.d_.m());
    impl::print_unit(s, "kg", v.d_.k());
    impl::print_unit(s, "s", v.d_.s());
    impl::print_unit(s, "C", v.d_.c());
    impl::print_unit(s, "K", v.d_.t());
    return s;
  }
};


} // namespace units
} // namespace vnix

#endif // ndef VNIX_UNITS_DIMVAL_HPP
