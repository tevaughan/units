/// @file       vnix/int-types.hpp
/// @brief      Definition of vnix::int_types.
/// @copyright  2019 Thomas E. Vaughan
/// @license    GPL Version 3 or later.

#ifndef VNIX_INT_TYPES_HPP
#define VNIX_INT_TYPES_HPP

#include <cstdint> // for uint_least8_t etc.

namespace vnix {


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


} // namespace vnix

#endif // ndef VNIX_INT_TYPES_HPP
