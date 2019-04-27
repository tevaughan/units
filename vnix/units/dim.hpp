/// @file       vnix/units/dim.hpp
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
enum base_off {
  TIM,      ///< Offset of exponent for time.
  LEN,      ///< Offset of exponent for length.
  MAS,      ///< Offset of exponent for mass.
  CHG,      ///< Offset of exponent for charge.
  TMP,      ///< Offset of exponent for temperature.
  NUM_BASES ///< Number of base quantities.
};


/// Composite dimension, which stores a rational exponent for each base
/// quantity.
class dim {
public:
  using rat = rational<4, 2>; ///< Type of rational for dimensioned values.

  /// Type of unsigned word in which dimensions are encoded.
  using word = int_types<NUM_BASES * rat::BITS>::US;

private:
  static_assert(NUM_BASES * rat::BITS <= 64, "too many bits for bases");
  word e_; ///< Storage for exponents.

  enum {
    /// Maximum number of bits to shift an encoding of a rational exponent.
    MAX_SHIFT = (NUM_BASES - 1) * rat::BITS
  };

  /// Encode exponent associated with highest base_off at highest bit-offsets.
  /// @tparam T  Type that is convertible rat.
  /// @tparam t  Exponent to be encoded.
  template <typename T> constexpr static word encode(T t) {
    return rat::encode(t) << MAX_SHIFT;
  }

  /// Encode exponents, with higher base_off at higher bit-offsets.
  /// @tparam T   Type that is convertible to rat.
  /// @tparam U   List of types, each convertible to rat.
  /// @param  t   Exponent to be encoded at lowest bit-offsets.
  /// @param  us  Exponents to be encoded at higher bit-offsets.
  template <typename T, typename... U>
  constexpr static word encode(T t, U... us) {
    constexpr auto N = sizeof...(us);
    static_assert(N < NUM_BASES, "too many exponents");
    constexpr auto SHIFT = MAX_SHIFT - N * rat::BITS;
    constexpr word MASK  = bit_range<word>(0, rat::BITS - 1) << SHIFT;
    word const     e     = rat::encode(t) << SHIFT;
    return (e & MASK) | (encode(us...) & ~MASK);
  }

public:
  /// Initialize from dim that has been encoded into a word.
  /// @param u  Encoded data for dim.
  constexpr explicit dim(word u) : e_(u) {}

  /// Initialize exponents, one for each base quantity.
  ///
  /// @tparam T   List of types, one for each initializer in list for
  ///             rat_array.  Each type should be rat.
  ///
  /// @param  ts  List of initializers for rat_array.
  template <typename... T> constexpr dim(T... ts) : e_(encode(ts...)) {
    static_assert(sizeof...(ts) == NUM_BASES, "illegal number of bases");
  }

  /// Encode data for this dim into a word.
  constexpr operator word() const { return e_; }

  /// Rational exponent at specified offset.
  /// @param off  Offset of exponent.
  constexpr rat exp(base_off off) const {
    constexpr auto mask = bit_range<word>(0, rat::BITS - 1);
    return rat::decode((e_ >> (off * rat::BITS)) & mask);
  }

  /// Rational exponent at specified offset.
  /// @param off  Offset of exponent.
  constexpr rat operator[](base_off off) const { return exp(off); }

  /// Set rational exponent at specified offset.
  /// @param off  Offset of exponent.
  /// @param r    Rational exponent.
  constexpr void set(base_off off, rat r) {
    unsigned const bit_off = off * rat::BITS;
    word const     expon   = rat::encode(r) << bit_off;
    auto const     mask    = bit_range<word>(0, rat::BITS - 1) << bit_off;
    e_                     = (e_ & ~mask) | (expon & mask);
  }

  /// Combine each exponent with corresponding other exponent via function.
  /// @tparam F  Type of function for combining two rationals to make third.
  /// @param  d  Other rational exponents.
  /// @param  f  Function operating on pair of rationals.
  /// @return    New rational exponents.
  template <typename F> constexpr dim combine(dim const &d, F f) const {
    dim r(word(0));
    for (unsigned b = 0; b < NUM_BASES; ++b) {
      auto const off = base_off(b);
      r.set(off, f(exp(off), d.exp(off)));
    }
    return r;
  }

  /// Transform each exponent according to a function.
  /// @tparam F  Type of function.
  /// @param  f  Unary function operating on rational.
  /// @return    New exponents.
  template <typename F> constexpr dim transform(F f) const {
    dim r(word(0));
    for (unsigned b = 0; b < NUM_BASES; ++b) {
      auto const off = base_off(b);
      r.set(off, f(exp(off)));
    }
    return r;
  }

  /// Function used to add corresponding exponents.
  constexpr static rat add(rat x, rat y) { return x + y; }

  /// Function used to subtract corresponding exponents.
  constexpr static rat sbtrct(rat x, rat y) { return x - y; }

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
  constexpr dim operator+(dim const &a) const { return combine(a, add); }

  /// Subtract corresponding exponents.
  /// This is called when one physical quantity is divided by another.
  /// Passing lambda in constexpr function requires C++17.
  /// @param s  Subtrahends.
  /// @return   Differences.
  constexpr dim operator-(dim const &s) const { return combine(s, sbtrct); }

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
  static std::ostream &print_unit(std::ostream &s, char const *u, rat e) {
    if (e.to_bool()) {
      s << " " << u;
      if (e != rat(1)) {
        s << "^";
        if (e.d() != 1) { s << "["; }
        s << e;
        if (e.d() != 1) { s << "]"; }
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
