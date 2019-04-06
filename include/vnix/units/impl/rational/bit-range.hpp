/// @file       units/impl/rational/bit-range.hpp
///
/// @brief      Definition of
///             vnix::units::impl::bit,
///             vnix::units::impl::bit_range.
///
/// @copyright  2019 Thomas E. Vaughan
/// @license    GPL3.

#ifndef VNIX_UNITS_IMPL_BIT_HPP
#define VNIX_UNITS_IMPL_BIT_HPP

namespace vnix {
namespace units {
namespace impl {


/// Word with specified bit set.
/// @tparam I  Type of integer word.
/// @param  n  Offset of bit in word.
template <typename I> constexpr I bit(unsigned n) { return I(1) << n; }


/// Word with specified range of bits set.
/// @tparam I   Type of integer word.
/// @param  n1  Offset of bit at one   end of range.
/// @param  n2  Offset of bit at other end of range.
template <typename I> constexpr I bit_range(unsigned n1, unsigned n2) {
  if (n1 < n2) {
    return bit<I>(n1) | bit_range<I>(n1 + 1, n2);
  }
  if (n2 < n1) {
    return bit<I>(n2) | bit_range<I>(n2 + 1, n1);
  }
  return bit<I>(n1);
}


} // namespace impl
} // namespace units
} // namespace vnix

#endif // ndef VNIX_UNITS_IMPL_BIT_HPP
