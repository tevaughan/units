/// @file       units/print-unit.hpp
/// @brief      Definition of vnix::units::impl::print_unit.
/// @copyright  2019  Thomas E. Vaughan
/// @license    GPL Version 3 or later.

#ifndef VNIX_UNITS_PRINT_UNIT_HPP
#define VNIX_UNITS_PRINT_UNIT_HPP

#include <iostream>
#include <vnix/rational.hpp>

namespace vnix {
namespace units {


/// Print to output stream the symbolic contribution from a given unit.
/// @param s  Output stream.
/// @param u  Abbreviation for unit.
/// @param e  Exponent of unit.
inline std::ostream &print_unit(std::ostream &s, char const *u, rat8_t e) {
  if (e.to_bool()) {
    s << " " << u;
    if (e != rat8_t(1)) {
      s << "^";
      if (e.d() != 1) {
        s << "[";
      }
      s << e;
      if (e.d() != 1) {
        s << "]";
      }
    }
  }
  return s;
}


} // namespace units
} // namespace vnix

#endif // ndef VNIX_UNITS_IMPL_PRINT_UNIT_HPP
