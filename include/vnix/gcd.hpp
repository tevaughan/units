/// @file       gcd.hpp
/// @brief      Definition of vnix::gcd.
/// @copyright  2019  Thomas E. Vaughan
/// @license    GPL Version 3 or later.

#ifndef VNIX_GCD_HPP
#define VNIX_GCD_HPP

#include <cstdint> // for uint64_t

/// Thomas E. Vaughan's public software.
namespace vnix {


/// Greatest common divisor of two unsigned numbers.
/// @param a  First  number.
/// @param b  Second number.
/// @return   Greatest common divisor.
constexpr uint64_t gcd(uint64_t a, uint64_t b) {
  if (b == 0) {
    return a;
  } else {
    return gcd(b, a % b);
  }
}


} // namespace vnix

#endif // ndef VNIX_GCD_HPP
