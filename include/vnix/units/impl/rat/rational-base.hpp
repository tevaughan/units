/// @file       units/impl/rat/rational-base.hpp
/// @brief      Definition of vnix::units::impl::rat::rational_base.
/// @copyright  2019 Thomas E. Vaughan
/// @license    GPL3.

#ifndef VNIX_UNITS_IMPL_RAT_RATIONAL_BASE_HPP
#define VNIX_UNITS_IMPL_RAT_RATIONAL_BASE_HPP

#include <vnix/units/impl/rat/bit-range.hpp>
#include <vnix/units/impl/rat/sgn.hpp>

namespace vnix {
namespace units {
namespace impl {
namespace rat {


/// Constants corresponding to unsigned word U in which are stored the
/// numerator and denominator of a rational number.
///
/// @tparam U  Type of unsigned word in which rational number is encoded.
template <typename U> class rational_base {
protected:
  using S = typename sgn<U>::type; ///< Signed type corresponding to U.
  enum {
    BITS     = sizeof(U) * 8,   ///< Total number of bits in word.
    DNM_BITS = BITS / 2 - 1,    ///< Nmbr of bits for positive denominator.
    NMR_BITS = BITS - DNM_BITS, ///< Nmbr of bits for 2's-complement numerator.
    DNM_MASK = bit_range<U>(0, DNM_BITS - 1),   ///< Mask for denominator.
    NMR_MASK = bit_range<U>(DNM_BITS, BITS - 1) ///< Mask for numerator.
  };
};


} // namespace rat
} // namespace impl
} // namespace units
} // namespace vnix

#endif // ndef VNIX_UNITS_IMPL_RAT_RATIONAL_BASE_HPP
