/// @file       rat/sgn.hpp
/// @brief      Definition of vnix::rat::sgn.
/// @copyright  2019  Thomas E. Vaughan
/// @license    GPL Versio 3 or later.

#ifndef VNIX_RAT_SGN_HPP
#define VNIX_RAT_SGN_HPP

#include <cstdint> // for uint64_t, etc.

namespace vnix {
namespace rat {


/// Class encoding signed integer type that corresponds to unsigned type.
/// Note that only specializations are used.
/// @tparam U  Type of unsigned integer word.
template <typename U> struct sgn;

/// Specialization of sgn for 64-bit integer.
template <> struct sgn<uint64_t> { using type = int64_t; };

/// Specialization of sgn for 32-bit integer.
template <> struct sgn<uint32_t> { using type = int32_t; };

/// Specialization of sgn for 16-bit integer.
template <> struct sgn<uint16_t> { using type = int16_t; };

/// Specialization of sgn for 8-bit integer.
template <> struct sgn<uint8_t> { using type = int8_t; };


} // namespace rat
} // namespace vnix

#endif // ndef VNIX_RAT_SGN_HPP
