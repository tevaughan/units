#ifndef VNIX_UNITS_IMPL_DIM_BASE_HPP
#define VNIX_UNITS_IMPL_DIM_BASE_HPP

#include <cstdint> // for int64_t

namespace vnix {
namespace units {
namespace impl {


/// Base for class dimension, which represents a set of exponents, one for each
/// fundamental unit of a dimensioned quantity.  This base class has public
/// functions that are, by way of protected inheritance, protected in class
/// dimension.  The base class is abstracted out so that the functions may be
/// easily tested.
class dim_base {
protected:
  enum dim_off {
    M = 0, ///< Bit-offset of exponent for meters.
    K = 1, ///< Bit-offset of exponent for kilograms.
    S = 2, ///< Bit-offset of exponent for seconds.
    C = 3, ///< Bit-offset of exponent for Coulombs.
    T = 4  ///< Bit-offset of exponent for Kelvins.
  };
  uint64_t e_; ///< Word holding each exponent.

public:
  /// Initialize each exponent.
  /// @param m  Exponent for meters.
  /// @param k  Exponent for kilograms.
  /// @param s  Exponent for seconds.
  /// @param c  Exponent for Coulombs.
  /// @param t  Exponent for Kelvins.
  constexpr dim_base(char m, char k, char s, char c, char t)
      : e_(put(m, M) | put(k, K) | put(s, S) | put(c, C) | put(t, T)) {}
  /// Extract exponent for specified byte-offset.
  /// @param off  Byte-offset of exponent.
  /// @return     Exponent at byte-offset.
  constexpr char get(dim_off off) const { return (e_ >> (off * 8)) & 0xFF; }
  /// Insert exponent into specified byte of word that is zero otherwise.
  /// @param v    Byte containing value of exponent.
  /// @param off  Offset of byte in word.
  /// @return     Initially zeroed word with specified byte set to specified
  ///             value.
  constexpr static uint64_t put(char v, dim_off off) {
    return uint64_t(uint8_t(v)) << (off * 8);
  }
};


} // namespace impl
} // namespace units
} // namespace vnix

#endif // ndef VNIX_UNITS_IMPL_DIM_BASE_HPP

