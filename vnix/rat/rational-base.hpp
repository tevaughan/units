/// @file       vnix/rat/rational-base.hpp
/// @brief      Definition of vnix::rat::rational_base.
/// @copyright  2019 Thomas E. Vaughan
/// @license    GPL Version 3 or later.

#ifndef VNIX_RAT_RATIONAL_BASE_HPP
#define VNIX_RAT_RATIONAL_BASE_HPP

#include <cstdint> // for uint_least8_t etc.
#include <vnix/bit-range.hpp>
#include <vnix/int-types.hpp>

namespace vnix {
namespace rat {


/// Constants and types associated with a rational number with a specified
/// number of bits for the numerator and a specified number of bits for the
/// denominator.
///
/// @tparam NNB  Number of bits for two's-complement numerator.
/// @tparam NDB  Number of bits for positive denominator.
template <unsigned NMR_BITS, unsigned DNM_BITS>
struct rational_base : public int_types<NMR_BITS + DNM_BITS> {
  /// Total number of bits used by rational number.
  enum {
    BITS = NMR_BITS + DNM_BITS ///< Total number of bits.
  };

  using typename int_types<BITS>::US; ///< Smallest acceptable unsigned type.

  /// Mask for each of numerator and denominator.
  enum {
    DNM_MASK = bit_range<US>(0, DNM_BITS - 1),   ///< Mask for denominator.
    NMR_MASK = bit_range<US>(DNM_BITS, BITS - 1) ///< Mask for numerator.
  };
};


} // namespace rat
} // namespace vnix

#endif // ndef VNIX_RAT_RATIONAL_BASE_HPP
