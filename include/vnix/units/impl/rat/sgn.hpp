/// @file       units/impl/rat/sgn.hpp
/// @brief      Definition of vnix;:units::impl::rat::sgn.
/// @copyright  2019 Thomas E. Vaughan
/// @license    GPL3.

#ifndef VNIX_UNITS_IMPL_RAT_SGN_HPP
#define VNIX_UNITS_IMPL_RAT_SGN_HPP

#include <cstdint> // for uint64_t, etc.

namespace vnix {
namespace units {
namespace impl {
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
} // namespace impl
} // namespace units
} // namespace vnix

#endif // ndef VNIX_UNITS_IMPL_RAT_SGN_HPP
