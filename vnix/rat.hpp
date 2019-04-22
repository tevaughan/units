/// @file       vnix/rat.hpp
/// @brief      Typedefs for class vnix::rat::rational.
/// @copyright  2019  Thomas E. Vaughan
/// @license    GPL Version 3 or later.

#ifndef VNIX_RATIONAL_HPP
#define VNIX_RATIONAL_HPP

#include <vnix/rat/rational.hpp>

namespace vnix {


/// Bring class rational into vnix namespace.
/// @tparam NB  Number of bits for numerator.
/// @tparam DB  Number of bits for denominator.
template <unsigned NB, unsigned DB> using rational = rat::rational<NB, DB>;


/// Rational with five bits for numerator and three for denominator.
using rat8_t = rational<5, 3>;

/// Rational with nine bits for numerator and seven for denominator.
using rat16_t = rational<9, 7>;

/// Rational with 17 bits for numerator and 15 for denominator.
using rat32_t = rational<17, 15>;

/// Rational with 33 bits for numerator and 31 for denominator.
using rat64_t = rational<33, 31>;


} // namespace vnix

#endif // ndef VNIX_RATIONAL_HPP
