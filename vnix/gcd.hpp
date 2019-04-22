/// @file       vnix/gcd.hpp
/// @brief      Definition of vnix::gcd.
/// @copyright  2019  Thomas E. Vaughan
/// @license    GPL Version 3 or later.

#ifndef VNIX_GCD_HPP
#define VNIX_GCD_HPP

#include <cstdint> // for uint64_t

/// Thomas E. Vaughan's public software.
namespace vnix {


/// Output type resulting from promition of two input types through modular
/// division.
///
/// @tparam U  One   input type.
/// @tparam V  Other input type.
template <typename U, typename V> using promoted = decltype(U() % V());


/// Greatest common divisor of two numbers.
/// @param a  First  number.
/// @param b  Second number.
/// @return   Greatest common divisor.
template <typename A, typename B> constexpr promoted<A, B> gcd(A a, B b) {
  if (b == 0) {
    return a;
  } else {
    return gcd(b, a % b);
  }
}


} // namespace vnix

#endif // ndef VNIX_GCD_HPP
