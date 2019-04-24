/// @file       vnix/rat/encoding.hpp
/// @brief      Definition of vnix::rat::encoding.
/// @copyright  2019 Thomas E. Vaughan
/// @license    GPL3.

#ifndef VNIX_RAT_ENCODING_HPP
#define VNIX_RAT_ENCODING_HPP

#include <vnix/bit-range.hpp>
#include <vnix/rat/normalized-pair.hpp>

namespace vnix {
namespace rat {


/// Encoding of numerator and denominator into unsigned word.
/// @tparam NMR_BITS  Number of bits for numerator.
/// @tparam DNM_BITS  Number of bits for denominator.
template <unsigned NMR_BITS, unsigned DNM_BITS> class encoding {
public:
  enum { /** Total number of bits. */ BITS = NMR_BITS + DNM_BITS };
  using utype = typename int_types<BITS>::US; ///< Unsigned type for encoding.
  using stype = typename int_types<BITS>::SS; ///< Signed   type for encoding.

  /// Mask for each of numerator and denominator.
  enum {
    DNM_MASK = bit_range<utype>(0, DNM_BITS - 1),   ///< Mask for denominator.
    NMR_MASK = bit_range<utype>(DNM_BITS, BITS - 1) ///< Mask for numerator.
  };

private:
  /// Calculated encoding from normalized numerator and denominator.
  /// @param p  Normalized numerator and denominator.
  constexpr static utype encode(normalized_pair<NMR_BITS, DNM_BITS> p) {
    utype const num_enc = utype(p.n()) << DNM_BITS;
    utype const den_enc = (p.d() - 1) & DNM_MASK;
    return num_enc | den_enc;
  }

protected:
  utype c_; ///< Unsigned word storing encoding.

  /// Allow descendant to construct from code-word.
  /// @param c  Code-word.
  constexpr encoding(utype c) : c_(c) {}

public:
  /// Initialize from normalized numerator and denominator.
  /// @param p  Normalized numerator and denominator.
  constexpr encoding(normalized_pair<NMR_BITS, DNM_BITS> p) : c_(encode(p)) {}

  /// Normalized numerator.
  constexpr typename int_types<NMR_BITS>::SF n() const {
    // Perform arithmetic right-shift on signed number.  With C++20, this could
    // be done simply as 'return stype(c_) >> DNM_BITS'.
    utype const sign_bit = c_ & bit<utype>(BITS - 1);
    utype const shifted  = c_ >> DNM_BITS;
    enum { UTYPE_BITS = 8 * sizeof(utype) };
    if (sign_bit && NMR_BITS < UTYPE_BITS) {
      return shifted | bit_range<utype>(NMR_BITS, UTYPE_BITS - 1);
    }
    return shifted;
  }

  /// Normalized denominator.
  constexpr typename int_types<DNM_BITS>::UF d() const {
    return (c_ & DNM_MASK) + 1;
  }
};


} // namespace rat
} // namespace vnix

#endif // ndef VNIX_RAT_ENCODING_HPP
