#ifndef VNIX_IMPL_PRINT_UNIT_HPP
#define VNIX_IMPL_PRINT_UNIT_HPP

#include <iostream>

namespace vnix {
namespace impl {

/// Print to output stream the symbolic contribution from a given unit.
/// @param s  Output stream.
/// @param u  Abbreviation for unit.
/// @param e  Exponent of unit.
inline std::ostream &print_unit(std::ostream &s, char const *u, int e) {
  if (e) {
    s << " " << u;
    if (e != 1) {
      s << "^" << e;
    }
  }
  return s;
}

} // namespace impl
} // namespace vnix

#endif // ndef VNIX_IMPL_PRINT_UNIT_HPP
