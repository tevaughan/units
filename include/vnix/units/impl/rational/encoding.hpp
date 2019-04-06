/// @file       units/impl/rational/encoding.hpp
/// @brief      Definition of vnix::units::impl::encoding.
/// @copyright  2019 Thomas E. Vaughan
/// @license    GPL3.

#ifndef VNIX_UNITS_IMPL_ENCODING_HPP
#define VNIX_UNITS_IMPL_ENCODING_HPP

#include <vnix/units/impl/rational/normalized-pair.hpp>

namespace vnix {
namespace units {
namespace impl {


/// Encoding of numerator and denominator for a rational number in unsigned
/// word of type U.
///
/// @tparam U  Type of unsigned word for encoding.
template <typename U> class encoding : public rational_base<U> {
  U c_; ///< Unsigned Word in which numerator and denominator are encoded.

  using typename rational_base<U>::S;
  using rational_base<U>::DNM_BITS;
  using rational_base<U>::NMR_MASK;
  using rational_base<U>::DNM_MASK;

public:
  /// Initialize encoding of rational number in unsigned word.
  /// @param p  Normalized numerator and denominator.
  constexpr encoding(normalized_pair<U> p)
      : c_(((p.n() << DNM_BITS) & NMR_MASK) | ((p.d() - 1) & DNM_MASK)) {}

  /// Normalized numerator.
  constexpr S n() const { return S(c_) >> DNM_BITS; }

  /// Normalized denominator.
  constexpr U d() const { return (c_ & DNM_MASK) + 1; }
};


} // namespace impl
} // namespace units
} // namespace vnix

#endif // ndef VNIX_UNITS_IMPL_ENCODING_HPP
