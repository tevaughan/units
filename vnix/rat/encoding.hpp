/// @file       vnix/rat/encoding.hpp
/// @brief      Definition of vnix::rat::encoding.
/// @copyright  2019 Thomas E. Vaughan
/// @license    GPL3.

#ifndef VNIX_RAT_ENCODING_HPP
#define VNIX_RAT_ENCODING_HPP

#include <vnix/rat/normalized-pair.hpp>

namespace vnix {
namespace rat {


/// Encoding of numerator and denominator into unsigned word.
/// @tparam NMR_BITS  Number of bits for numerator.
/// @tparam DNM_BITS  Number of bits for denominator.
template <unsigned NMR_BITS, unsigned DNM_BITS>
class encoding : public rational_base<NMR_BITS, DNM_BITS> {
  using P = rational_base<NMR_BITS, DNM_BITS>;

protected:
  using US = typename P::US;
  using UF = typename P::UF;
  using SS = typename P::SS;
  using SF = typename P::SF;

  /// Unsigned word in which numerator and denominator are encoded.
  US c_;

  using P::DNM_MASK;
  using P::NMR_MASK;

  /// Construct from code-word.
  /// @param c  Code-word.
  constexpr encoding(UF c) : c_(c) {}

public:
  using type = US; ///< Type of unsigned word for encoding.

  /// Initialize encoding of rational number in unsigned word.
  /// @param p  Normalized numerator and denominator.
  constexpr encoding(normalized_pair<NMR_BITS, DNM_BITS> p)
      : c_((US(p.n()) << DNM_BITS) | ((p.d() - 1) & DNM_MASK)) {}

  /// Normalized numerator.
  constexpr SF n() const { return SS(c_) >> DNM_BITS; }

  /// Normalized denominator.
  constexpr UF d() const { return (c_ & DNM_MASK) + 1; }
};


} // namespace rat
} // namespace vnix

#endif // ndef VNIX_RAT_ENCODING_HPP
