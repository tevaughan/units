/// @file       vnix/rat.hpp
/// @brief      Typedefs for class rational.
/// @copyright  2019  Thomas E. Vaughan
/// @license    GPL Version 3 or later.

#ifndef VNIX_RATIONAL_HPP
#define VNIX_RATIONAL_HPP

#include <vnix/rat/rational.hpp>

namespace vnix {


/// Bring class rational into vnix namespace.
/// @tparam U  Type of unsigned integer into which rational is encoded.
template <typename U> using rational = rat::rational<U>;


/// Rational with five bits for numerator and three for denominator.
using rat8_t = rational<uint8_t>;

/// Rational with nine bits for numerator and seven for denominator.
using rat16_t = rational<uint16_t>;

/// Rational with 17 bits for numerator and 15 for denominator.
using rat32_t = rational<uint32_t>;

/// Rational with 33 bits for numerator and 31 for denominator.
using rat64_t = rational<uint64_t>;


} // namespace vnix

#endif // ndef VNIX_RATIONAL_HPP
