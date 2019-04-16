/// @file       units/dim.hpp
/// @brief      Definition of vnix::units::dim.
/// @copyright  2019  Thomas E. Vaughan
/// @license    GPL Version 3 or later.

#ifndef VNIX_UNITS_DIM_HPP
#define VNIX_UNITS_DIM_HPP

#include <array>        // for array
#include <vnix/rat.hpp> // for rational

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


using rat = rat8_t; ///< Type of rational for dimensioned values.


/// Composite dimension, which stores a rational exponent for each base
/// quantity.
class dim {
  static_assert(NUM_BASES <= sizeof(uint64_t), "too many bases");
  std::array<rat, NUM_BASES> e_; ///< Storage for exponents.

public:
  /// Initialize exponents, one for each base quantity.
  /// @param e  Array containing exponents.
  constexpr dim(std::array<rat, NUM_BASES> e) : e_{e} {}

  /// Initialize from dim that has been encoded into a uint64_t.
  /// @param u  Encoded data for dim.
  constexpr explicit dim(uint64_t u)
      : e_{rat::decode((u >> (TIM * 8)) & 0xFF),
           rat::decode((u >> (LEN * 8)) & 0xFF),
           rat::decode((u >> (MAS * 8)) & 0xFF),
           rat::decode((u >> (CHG * 8)) & 0xFF),
           rat::decode((u >> (TMP * 8)) & 0xFF)} {}

  /// Encode data for this dim into a uint64_t.
  constexpr operator uint64_t() const {
    return (uint64_t(rat::encode(e_[TIM])) << (TIM * 8)) |
           (uint64_t(rat::encode(e_[LEN])) << (LEN * 8)) |
           (uint64_t(rat::encode(e_[MAS])) << (MAS * 8)) |
           (uint64_t(rat::encode(e_[CHG])) << (CHG * 8)) |
           (uint64_t(rat::encode(e_[TMP])) << (TMP * 8));
  }

  /// Reference to immutable rational exponent at specified offset.
  /// @param off  Offset of exponent.
  constexpr rat const &operator[](base_off off) const { return e_[off]; }

  /// Reference to mutable rational exponent at specified offset.
  /// @param off  Offset of exponent.
  constexpr rat &operator[](base_off off) { return e_[off]; }

  /// Reference to immutable array of exponents.
  constexpr std::array<rat, NUM_BASES> const &exponents() const { return e_; }

  /// Combine each exponent with corresponding other exponent via function.
  /// @tparam F  Type of function.
  /// @param  d  Other exponents.
  /// @param  f  Binary function operating on pair of rationals.
  /// @return    New exponents.
  template <typename F> constexpr dim combine(dim const &d, F f) const {
    dim  r = *this;
    auto j = d.e_.begin();
    for (auto i = r.e_.begin(); i != r.e_.end();) {
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
    for (auto i = r.e_.begin(); i != r.e_.end(); ++i) {
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
  /// @return   Products.
  constexpr dim operator*(rat f) const {
    return transform([f](rat x) { return x * f; });
  }

  /// Divide exponents by rational factor.
  /// This is called when a physical quantity is raised to a power.
  /// @param f  Factor.
  /// @return   Products.
  constexpr dim operator/(rat f) const {
    return transform([f](rat x) { return x / f; });
  }

  /// Print to output stream the symbolic contribution from a given unit.
  /// @param s  Output stream.
  /// @param u  Abbreviation for unit.
  /// @param e  Exponent of unit.
  static std::ostream &print_unit(std::ostream &s, char const *u, rat8_t e) {
    if (e.to_bool()) {
      s << " " << u;
      if (e != rat8_t(1)) {
        s << "^";
        if (e.d() != 1) {
          s << "[";
        }
        s << e;
        if (e.d() != 1) {
          s << "]";
        }
      }
    }
    return s;
  }

  /// Print to to output stream.
  /// @param s  Reference to stream.
  /// @param d  Dimension to print.
  friend std::ostream &operator<<(std::ostream &s, dim d) {
    print_unit(s, "m", d[LEN]);
    print_unit(s, "kg", d[MAS]);
    print_unit(s, "s", d[TIM]);
    print_unit(s, "C", d[CHG]);
    print_unit(s, "K", d[TMP]);
    return s;
  }
};


constexpr dim nul_dim({0, 0, 0, 0, 0}); ///< Dimensionless.
constexpr dim tim_dim({1, 0, 0, 0, 0}); ///< Dimension of time.
constexpr dim len_dim({0, 1, 0, 0, 0}); ///< Dimension of length.
constexpr dim mas_dim({0, 0, 1, 0, 0}); ///< Dimension of mass.
constexpr dim chg_dim({0, 0, 0, 1, 0}); ///< Dimension of charge.
constexpr dim tmp_dim({0, 0, 0, 0, 1}); ///< Dimension of temperature.


} // namespace units
} // namespace vnix

#endif // ndef VNIX_DIM_HPP
