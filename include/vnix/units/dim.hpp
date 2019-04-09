/// @file       units/dim.hpp
/// @brief      Definition of vnix::units::dim.
/// @copyright  2019 Thomas E. Vaughan
/// @license    GPL, Version 3 or later.

#ifndef VNIX_UNITS_DIM_HPP
#define VNIX_UNITS_DIM_HPP

#include <vnix/units/impl/rational.hpp> // for rational

namespace vnix {
namespace units {


/// Offset of exponent for each base quantity.
enum base_off : uint8_t {
  TIM,      ///< Offset of exponent for time.
  LEN,      ///< Offset of exponent for length.
  MAS,      ///< Offset of exponent for mass.
  CHG,      ///< Offset of exponent for charge.
  TMP,      ///< Offset of exponent for temperature.
  NUM_BASES ///< Number of base quantities.
};


// Forward declaration so that dimval can be declared friend of dim.
template <uint64_t D> class dimval;

using rat = impl::rat8_t; ///< Type of rational for dimensioned values.

/// Composite dimension, which stores a rational exponent for each base
/// quantity.
class dim {
  static_assert(NUM_BASES <= sizeof(uint64_t), "too many bases");
  rat e_[NUM_BASES]; ///< Storage for exponents.

  /// dimval needs to be a friend so that it can convert from uint64_t to dim.
  template <uint64_t D> friend class dimval;

  /// Get rational exponent out of uint64_t word.
  /// dim must be friend of rat so that rat can be constructed from uint8_t.
  /// @param e    Word.
  /// @param off  Offset of exponent.
  constexpr static rat get(uint64_t e, base_off off) {
    return rat::code((e >> (off * 8)) & 0xFF);
  }

  /// Put rational exponent into its place in uint64_t word, zero elsewhere.
  /// dim must be friend of rat so that code can be extracted from rat.
  /// @param off  Offset of exponent.
  constexpr uint64_t put(base_off off) const {
    return uint64_t(e_[off].c_) << (off * 8);
  }

  /// Initialize exponents, one for each base quantity.
  /// @param e  Encoded in uint64_t, elements of rational type.
  constexpr explicit dim(uint64_t e)
      : e_{get(e, TIM), get(e, LEN), get(e, MAS), get(e, CHG), get(e, TMP)} {}

  /// Pointer to first rational exponent.
  constexpr rat const *begin() const { return e_; }

  /// Pointer to rational exponent just past last.
  constexpr rat const *end() const { return e_ + NUM_BASES; }

  /// Pointer to first rational exponent.
  constexpr rat *begin() { return e_; }

  /// Pointer to rational exponent just past last.
  constexpr rat *end() { return e_ + NUM_BASES; }

public:
  /// Initialize exponents, one for each base quantity.
  /// @param tim  Exponent of time       -dimension.
  /// @param len  Exponent of length     -dimension.
  /// @param mas  Exponent of mass       -dimension.
  /// @param chg  Exponent of charge     -dimension.
  /// @param tmp  Exponent of temperature-dimension.
  constexpr dim(rat tim, rat len, rat mas, rat chg, rat tmp)
      : e_{tim, len, mas, chg, tmp} {}

  /// Exponents encoded in uint64_t.
  /// This needs to be public so that a declaration can use a dim as
  /// a template-value parameter.
  constexpr operator uint64_t() const {
    return put(TIM) | put(LEN) | put(MAS) | put(CHG) | put(TMP);
  }

  /// Reference to immutable rational exponent at specified offset.
  /// @param off  Offset of exponent.
  constexpr rat const &operator[](base_off off) const { return e_[off]; }

  /// Reference to mutable rational exponent at specified offset.
  /// @param off  Offset of exponent.
  constexpr rat &operator[](base_off off) { return e_[off]; }

  /// Combine each exponent with corresponding other exponent via function.
  /// @tparam F  Type of function.
  /// @param  d  Other exponents.
  /// @param  f  Binary function operating on pair of rationals.
  /// @return    New exponents.
  template <typename F> constexpr dim combine(dim const &d, F f) const {
    dim  r = *this;
    auto j = d.begin();
    for (auto i = r.begin(); i != r.end();) {
      f(*i++, *j++);
    }
    return r;
  }

  /// Transform each exponent according to a function.
  /// @tparam F  Type of function.
  /// @param  f  Unary function operating on rational.
  /// @return    New exponents.
  template <typename F> constexpr dim transform(F f) const {
    dim r = *this;
    for (auto i = r.begin(); i != r.end(); ++i) {
      *i = f(*i);
    }
    return r;
  }

  /// Add corresponding exponents.
  /// This is called when two physical quantities are multiplied.
  /// Passing lambda in constexpr function requires C++17.
  /// @param a  Addends.
  /// @return   Sums.
  constexpr dim operator+(dim const &a) const {
    return combine(a, [](rat &x, rat y) { x += y; });
  }

  /// Subtract corresponding exponents.
  /// This is called when one physical quantity is divided by another.
  /// Passing lambda in constexpr function requires C++17.
  /// @param s  Subtrahends.
  /// @return   Differences.
  constexpr dim operator-(dim const &s) const {
    return combine(s, [](rat &x, rat y) { x -= y; });
  }

  /// Multiply exponents by rational factor.
  /// This is called when a physical quantity is raised to a power.
  /// @param f  Factor.
  /// @param p  Products.
  constexpr dim operator*(rat f) const {
    return transform([f](rat x) { return x * f; });
  }

  /// Divide exponents by rational factor.
  /// This is called when a physical quantity is raised to a power.
  /// @param f  Factor.
  /// @param p  Products.
  constexpr friend dim operator/(dim d, rat f) {
    return d.transform([f](rat x) { return x / f; });
  }
};

constexpr dim null(0, 0, 0, 0, 0); ///< Indicator of dimensionless quantity.

} // namespace units
} // namespace vnix

#endif // ndef VNIX_DIM_HPP
