/// @file       vnix/gcd.hpp
/// @brief      Definition of vnix::gcd.
/// @copyright  2019 Thomas E. Vaughan; all rights reserved.
/// @license    BSD Three-Clause; see LICENSE.

#ifndef VNIX_GCD_HPP
#define VNIX_GCD_HPP

/// Thomas E. Vaughan's public software.
namespace vnix {


/// Output-type resulting from modular division of input-types.
/// @tparam U  One   input type.
/// @tparam V  Other input type.
template <typename U, typename V> using gcd_promoted = decltype(U() % V());


namespace impl {


/// Greatest common divisor of two nonnegative numbers.
/// @param a  First  nonnegative number.
/// @param b  Second nonnegative number.
/// @return   Greatest common divisor.
template <typename A, typename B>
constexpr gcd_promoted<A, B> basic_gcd(A a, B b) {
  if (b == 0) { return a; }
  return basic_gcd(b, a % b);
}


} // namespace impl


/// Greatest common divisor of two numbers; only absolute values are used.
/// @param a  First  number.
/// @param b  Second number.
/// @return   Greatest common divisor.
template <typename A, typename B> constexpr gcd_promoted<A, B> gcd(A a, B b) {
  if (a < 0) { a = -a; }
  if (b == 0) { return a; }
  if (b < 0) { b = -b; }
  return impl::basic_gcd(b, a % b);
}


} // namespace vnix

#endif // ndef VNIX_GCD_HPP
