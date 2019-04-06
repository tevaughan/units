/// @file       units/impl/rat/gcd.hpp
/// @brief      Definition of vnix::units::impl::rat::gcd.
/// @copyright  2019 Thomas E. Vaughan
/// @license    GPL3.

#ifndef VNIX_UNITS_IMPL_RAT_GCD_HPP
#define VNIX_UNITS_IMPL_RAT_GCD_HPP

#include <cstdint> // for uint64_t

namespace vnix {
namespace units {
namespace impl {
namespace rat {


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


} // namespace rat
} // namespace impl
} // namespace units
} // namespace vnix

#endif // ndef VNIX_UNITS_IMPL_RAT_GCD_HPP
