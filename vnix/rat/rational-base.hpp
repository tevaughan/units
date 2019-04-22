/// @file       vnix/rat/rational-base.hpp
/// @brief      Definition of vnix::rat::rational_base.
/// @copyright  2019 Thomas E. Vaughan
/// @license    GPL Version 3 or later.

#ifndef VNIX_RAT_RATIONAL_BASE_HPP
#define VNIX_RAT_RATIONAL_BASE_HPP

#include <cstdint> // for uint_least8_t etc.
#include <vnix/bit-range.hpp>

namespace vnix {
namespace rat {


/// Smallest integer types for holding certain number of bits.
///
/// Generic int_types refers to the int_types with the next-larger number of
/// bits.  The recursion ends with a terminal specialization: int_types<8>,
/// int_types<16>, int_types<32>, or int_types<64>.
///
/// @tparam NB  Number of bits.
template <unsigned NB> struct int_types {
  static_assert(NB <= 64, "Word must not require more than 64 bits.");
  using US = typename int_types<NB + 1>::US; ///< Smallest unsigned integer.
  using SS = typename int_types<NB + 1>::SS; ///< Smallest   signed integer.
  using UF = typename int_types<NB + 1>::UF; ///< Fastest unsigned integer.
  using SF = typename int_types<NB + 1>::SF; ///< Fastest   signed integer.
};

/// Terminal specialization of int_types for eight-bit integer.
template <> struct int_types<8> {
  using US = uint_least8_t; ///< Smallest unsigned integer.
  using SS = int_least8_t;  ///< Smallest   signed integer.
  using UF = uint_fast8_t;  ///< Fastest unsigned integer.
  using SF = int_fast8_t;   ///< Fastest   signed integer.
};

/// Terminal specialization of int_types for 16-bit integer.
template <> struct int_types<16> {
  using US = uint_least16_t; ///< Smallest unsigned integer.
  using SS = int_least16_t;  ///< Smallest   signed integer.
  using UF = uint_fast16_t;  ///< Fastest unsigned integer.
  using SF = int_fast16_t;   ///< Fastest   signed integer.
};

/// Terminal specialization of int_types for 32-bit integer.
template <> struct int_types<32> {
  using US = uint_least32_t; ///< Smallest unsigned integer.
  using SS = int_least32_t;  ///< Smallest   signed integer.
  using UF = uint_fast32_t;  ///< Fastest unsigned integer.
  using SF = int_fast32_t;   ///< Fastest   signed integer.
};

/// Terminal specialization of int_types for 64-bit integer.
template <> struct int_types<64> {
  using US = uint_least64_t; ///< Smallest unsigned integer.
  using SS = int_least64_t;  ///< Smallest   signed integer.
  using UF = uint_fast64_t;  ///< Fastest unsigned integer.
  using SF = int_fast64_t;   ///< Fastest   signed integer.
};


/// Constants and types associated with a rational number with a specified
/// number of bits for the numerator and a specified number of bits for the
/// denominator.
///
/// @tparam NNB  Number of bits for two's-complement numerator.
/// @tparam NDB  Number of bits for positive denominator.
template <unsigned NMR_BITS, unsigned DNM_BITS>
struct rational_base : public int_types<NMR_BITS + DNM_BITS> {
  enum {
    BITS = NMR_BITS + DNM_BITS ///< Total number of bits.
  };
  using P = int_types<BITS>; ///< Type of parent.
  enum {
    /// Mask for denominator.
    DNM_MASK = bit_range<typename P::US>(0, DNM_BITS - 1),
    /// Mask for numerator.
    NMR_MASK = bit_range<typename P::US>(DNM_BITS, BITS - 1)
  };
};


} // namespace rat
} // namespace vnix

#endif // ndef VNIX_RAT_RATIONAL_BASE_HPP
