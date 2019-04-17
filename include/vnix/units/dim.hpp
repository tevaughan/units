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


/// Array of rational exponents.
struct rat_array {
  rat a[NUM_BASES]; ///< Storage for rational exponents.
};


/// Composite dimension, which stores a rational exponent for each base
/// quantity.
class dim {
  static_assert(NUM_BASES <= sizeof(uint64_t), "too many bases");
  rat_array            e_; ///< Storage for exponents.
  constexpr rat *      begin() { return e_.a; }
  constexpr rat *      end() { return e_.a + NUM_BASES; }
  constexpr rat const *begin() const { return e_.a; }
  constexpr rat const *end() const { return e_.a + NUM_BASES; }

public:
  /// Initialize exponents, one for each base quantity.
  ///
  /// @tparam T   List of types, one for each initializer in list for
  ///             rat_array.  Each type should be rat.
  ///
  /// @param  ts  List of initializers for rat_array.
  template <typename... T> constexpr dim(T... ts) : e_{ts...} {}

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
    return (uint64_t(rat::encode(e_.a[TIM])) << (TIM * 8)) |
           (uint64_t(rat::encode(e_.a[LEN])) << (LEN * 8)) |
           (uint64_t(rat::encode(e_.a[MAS])) << (MAS * 8)) |
           (uint64_t(rat::encode(e_.a[CHG])) << (CHG * 8)) |
           (uint64_t(rat::encode(e_.a[TMP])) << (TMP * 8));
  }

  /// Reference to immutable rational exponent at specified offset.
  /// @param off  Offset of exponent.
  constexpr rat const &operator[](base_off off) const { return e_.a[off]; }

  /// Reference to mutable rational exponent at specified offset.
  /// @param off  Offset of exponent.
  constexpr rat &operator[](base_off off) { return e_.a[off]; }

  /// Reference to immutable array of exponents.
  constexpr rat_array const &exponents() const { return e_; }

  /// Combine each exponent with corresponding other exponent via function.
  /// @tparam F  Type of function.
  /// @param  d  Other exponents.
  /// @param  f  Binary function operating on pair of rationals.
  /// @return    New exponents.
  template <typename F> constexpr dim combine(dim const &d, F f) const {
    dim        r    = *this;
    rat const *j    = d.begin();
    rat *const rend = r.end();
    for (rat *i = r.begin(); i != rend;) {
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

  /// Function used to add corresponding exponents.
  /// @param x  Reference incremented by corresponding exponent.
  /// @param y  Corresponding exponent.
  constexpr static void accum(rat &x, rat y) { x += y; }

  /// Function used to subtract corresponding exponents.
  /// @param x  Reference decremented by corresponding exponent.
  /// @param y  Corresponding exponent.
  constexpr static void decrm(rat &x, rat y) { x -= y; }

  /// Function object used for multiplying every exponent by a single factor.
  struct mult {
    rat f; ///< Factor by which to multiply exponents.

    /// Initialize from factor by which to multiply exponents.
    /// @param ff  Factor.
    constexpr mult(rat ff) : f(ff) {}

    /// Multiply an exponent by the factor.
    /// @param x  Input  exponent.
    /// @return   Output exponent.
    constexpr rat operator()(rat x) const { return x * f; }
  };

  /// Function object used for dividing every exponent by a single factor.
  struct divd {
    rat f; ///< Factor by which to divide exponents.

    /// Initialize from factor by which to divide exponents.
    /// @param ff  Factor.
    constexpr divd(rat ff) : f(ff) {}

    /// Divide an exponent by the factor.
    /// @param x  Input  exponent.
    /// @return   output exponent.
    constexpr rat operator()(rat x) const { return x / f; }
  };

  /// Add corresponding exponents.
  /// This is called when two physical quantities are multiplied.
  /// Passing lambda in constexpr function requires C++17.
  /// @param a  Addends.
  /// @return   Sums.
  constexpr dim operator+(dim const &a) const { return combine(a, accum); }

  /// Subtract corresponding exponents.
  /// This is called when one physical quantity is divided by another.
  /// Passing lambda in constexpr function requires C++17.
  /// @param s  Subtrahends.
  /// @return   Differences.
  constexpr dim operator-(dim const &s) const { return combine(s, decrm); }

  /// Multiply exponents by rational factor.
  /// This is called when a physical quantity is raised to a power.
  /// @param f  Factor.
  /// @return   Products.
  constexpr dim operator*(rat f) const { return transform(mult(f)); }

  /// Divide exponents by rational factor.
  /// This is called when a physical quantity is raised to a power.
  /// @param f  Factor.
  /// @return   Products.
  constexpr dim operator/(rat f) const { return transform(divd(f)); }

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
