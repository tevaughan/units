/// @file       vnix/rat/encoding.hpp
/// @brief      Definition of vnix::rat::encoding.
/// @copyright  2019 Thomas E. Vaughan
/// @license    GPL3.

#ifndef VNIX_RAT_ENCODING_HPP
#define VNIX_RAT_ENCODING_HPP

#include <vnix/rat/normalized-pair.hpp>
#include <vnix/rat/rational-base.hpp>

namespace vnix {
namespace rat {


/// Encoding of numerator and denominator into unsigned word.
/// @tparam NMR_BITS  Number of bits for numerator.
/// @tparam DNM_BITS  Number of bits for denominator.
template <unsigned NMR_BITS, unsigned DNM_BITS>
class encoding : public rational_base<NMR_BITS, DNM_BITS> {
  using P = rational_base<NMR_BITS, DNM_BITS>; ///< Type of parent.

public:
  using type = typename P::US; ///< Type of unsigned word for encoding.

protected:
  type c_;           ///< Unsigned word storing encoding.
  using P::DNM_MASK; ///< Mask for denominator in encoded word.

  /// Allow descendant to construct from code-word.
  /// @param c  Code-word.
  constexpr encoding(typename P::UF c) : c_(c) {}

public:
  /// Initialize from normalized numerator and denominator.
  /// @param p  Normalized numerator and denominator.
  constexpr encoding(normalized_pair<NMR_BITS, DNM_BITS> p)
      : c_((type(p.n()) << DNM_BITS) | ((p.d() - 1) & DNM_MASK)) {}

  /// Normalized numerator.
  constexpr typename int_types<NMR_BITS>::SF n() const {
    // Cast unsigned code-word to corresponding signed type so that right-shift
    // will pad with ones if numerator be negative.  This is necessary because
    // shifted value at the end needs to be negative in that case.
    return typename P::SS(c_) >> DNM_BITS;
  }

  /// Normalized denominator.
  constexpr typename int_types<DNM_BITS>::UF d() const {
    return (c_ & DNM_MASK) + 1;
  }
};


} // namespace rat
} // namespace vnix

#endif // ndef VNIX_RAT_ENCODING_HPP
