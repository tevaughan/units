/// @file       units/impl/dim-base.hpp
/// @brief      Definition of vnix::units::impl::dim_base.
/// @copyright  2019 Thomas E. Vaughan
/// @license    GPL3.

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
  /// Initialize each exponent, whose legal value is any whole or half integer.
  /// @param m  Exponent for meters.
  /// @param k  Exponent for kilograms.
  /// @param s  Exponent for seconds.
  /// @param c  Exponent for Coulombs.
  /// @param t  Exponent for Kelvins.
  constexpr dim_base(float m, float k, float s, float c, float t)
      : e_(put(m, M) | put(k, K) | put(s, S) | put(c, C) | put(t, T)) {}
  /// Extract exponent for specified byte-offset.
  /// @param off  Byte-offset of exponent.
  /// @return     Exponent at byte-offset.
  constexpr float get(dim_off off) const {
    return 0.5f * int8_t((e_ >> (off * 8)) & 0xFF);
  }
  /// Insert exponent into specified byte of word that is zero otherwise.
  /// @param v    Value of exponent.
  /// @param off  Offset of byte in word.
  /// @return     Initially zeroed word with specified byte set to specified
  ///             value.
  constexpr static uint64_t put(float v, dim_off off) {
    int8_t const rv = int8_t(2.0f * (v > 0 ? v + 0.25 : v - 0.25));
    return uint64_t(uint8_t(rv)) << (off * 8);
  }
};


} // namespace impl
} // namespace units
} // namespace vnix

#endif // ndef VNIX_UNITS_IMPL_DIM_BASE_HPP

