/// @file       vnix/units/dim.hpp
/// @brief      Definition of vnix::units::dim.
/// @copyright  2019 Thomas E. Vaughan; all rights reserved.
/// @license    BSD Three-Clause; see LICENSE.

#ifndef VNIX_UNITS_DIM_HPP
#define VNIX_UNITS_DIM_HPP

#include <array>                       // for array
#include <vnix/rat.hpp>                // for rational
#include <vnix/units/dim-base-off.hpp> // for dim_base_off

namespace vnix {
namespace units {


/// Rational exponent for each basis-element of dimension.
///
/// The template-type parameter DBO is of type dim_base_offset, which is
/// automatically generated from YAML and ERB before the header-only library is
/// installed.
///
/// @tparam DBO  Type of offset of basis-element of dimension.
template <typename DBO> class basic_dim {
public:
  using rat = rational<4, 2>; ///< Type of rational for dimensioned values.
  using off = DBO;            ///< Type of offset for each base-element.

  enum {
    NUM_BASES = DBO::num_offs,        ///< Number of bases for dimension.
    NUM_BITS  = NUM_BASES * rat::BITS ///< Number of bits in instance of dim.
  };

  /// Type of unsigned word in which dimensions are encoded.
  using word = typename int_types<NUM_BITS>::US;

private:
  static_assert(NUM_BITS <= 64, "too many bits for bases");
  word e_; ///< Storage for exponents.

  enum {
    /// Maximum number of bits to shift an encoding of a rational exponent.
    MAX_SHIFT = (NUM_BASES - 1) * rat::BITS
  };

  /// Encode exponent associated with highest DBO at highest bit-offsets.
  /// @tparam T  Type that is convertible rat.
  /// @tparam t  Exponent to be encoded.
  template <typename T> constexpr static word encode(T t) {
    return rat::encode(t) << MAX_SHIFT;
  }

#if 1
  /// Encode exponents, with higher DBO at higher bit-offsets.
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
#endif

  constexpr static word encode(std::array<rat, NUM_BASES> const &a) {
    word es = 0;
    for (uint_fast8_t i = 0; i < NUM_BASES; ++i) {
      auto const SHIFT = i * rat::BITS;
      word const MASK  = bit_range<word>(0, rat::BITS - 1) << SHIFT;
      word const e     = rat::encode(a[i]) << SHIFT;
      es |= (e & MASK);
    }
    return es;
  }

public:
  constexpr basic_dim() : e_(0) {}

  /// Initialize from dim that has been encoded into a word.
  /// By default, initialize null (dimensionless) dim.
  /// @param u  Encoded data for dim.
  constexpr explicit basic_dim(word u) : e_(u) {}

#if 1
  /// Initialize exponents, one for each base quantity.
  ///
  /// @tparam T   List of types, one for each initializer in list for
  ///             rat_array.  Each type should be rat.
  ///
  /// @param  ts  List of initializers for rat_array.
  template <typename... T> constexpr basic_dim(T... ts) : e_(encode(ts...)) {
    static_assert(sizeof...(ts) == NUM_BASES, "illegal number of bases");
  }
#endif

  constexpr basic_dim(std::array<rat, NUM_BASES> const &a) : e_(encode(a)) {}

  /// Encode data for this dim into a word.
  constexpr word encode() const { return e_; }

  /// Rational exponent at specified offset.
  /// @param off  Offset of exponent.
  constexpr rat exp(DBO off) const {
    constexpr auto mask = bit_range<word>(0, rat::BITS - 1);
    return rat::decode((e_ >> (off * rat::BITS)) & mask);
  }

  /// Rational exponent at specified offset.
  /// @param off  Offset of exponent.
  constexpr rat operator[](DBO off) const { return exp(off); }

  /// Set rational exponent at specified offset.
  /// @param off  Offset of exponent.
  /// @param r    Rational exponent.
  constexpr void set(DBO off, rat r) {
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
  template <typename F>
  constexpr basic_dim combine(basic_dim const &d, F f) const {
    basic_dim r(word(0));
    for (auto b : off::array) {
      auto const os = DBO(b);
      r.set(os, f(exp(os), d.exp(os)));
    }
    return r;
  }

  /// Transform each exponent according to a function.
  /// @tparam F  Type of function.
  /// @param  f  Unary function operating on rational.
  /// @return    New exponents.
  template <typename F> constexpr basic_dim transform(F f) const {
    basic_dim r(word(0));
    for (auto b : off::array) {
      auto const os = DBO(b);
      r.set(os, f(exp(os)));
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
  constexpr basic_dim operator+(basic_dim const &a) const {
    return combine(a, add);
  }

  /// Subtract corresponding exponents.
  /// This is called when one physical quantity is divided by another.
  /// Passing lambda in constexpr function requires C++17.
  /// @param s  Subtrahends.
  /// @return   Differences.
  constexpr basic_dim operator-(basic_dim const &s) const {
    return combine(s, sbtrct);
  }

  /// Multiply exponents by rational factor.
  /// This is called when a physical quantity is raised to a power.
  /// @param f  Factor.
  /// @return   Products.
  constexpr basic_dim operator*(rat f) const { return transform(mult(f)); }

  /// Divide exponents by rational factor.
  /// This is called when a physical quantity is raised to a power.
  /// @param f  Factor.
  /// @return   Products.
  constexpr basic_dim operator/(rat f) const { return transform(divd(f)); }

  /// Divide exponents by rational factor.
  /// This is called when a physical quantity is raised to a power.
  /// @param f  Factor.
  /// @return   Products.
  constexpr basic_dim operator/(rat::stype f) const {
    return transform(divd(f));
  }

  constexpr bool operator==(basic_dim const &d) const { return e_ == d.e_; }

  constexpr bool operator!=(basic_dim const &d) const { return e_ != d.e_; }

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
  friend std::ostream &operator<<(std::ostream &s, basic_dim d) {
    for (auto i : DBO::array) { print_unit(s, DBO::sym[i], d[i]); }
    return s;
  }
};


/// Define dim as specification of basic_dim by way of YAML- and ERB-generated
/// type dim_base_off.
using dim = basic_dim<dim_base_off>;


constexpr static dim nul_dim; ///< Null dimension.


} // namespace units
} // namespace vnix

#endif // ndef VNIX_DIM_HPP
