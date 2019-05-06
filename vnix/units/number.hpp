/// @file       vnix/units/number.hpp
/// @brief      Definition of vnix::units::number.
/// @copyright  2019 Thomas E. Vaughan; all rights reserved.
/// @license    BSD Three-Clause; see LICENSE.

#ifndef VNIX_UNITS_NUMBER_HPP
#define VNIX_UNITS_NUMBER_HPP

#include <cstdlib>

namespace Eigen {

template <typename S, int R, int C, int OPT, int MR, int MC> class Matrix;
template <typename B, typename L, typename R> class CwiseBinaryOp;
template <typename L, typename R, int OPT> class Product;

} // namespace Eigen


namespace vnix {

namespace mv {
template <typename T, size_t NR, size_t NC> struct mat;
}

namespace units {


/// Base-struct for wrapper to disambiguate scalar from dimval and protected
/// storage for numeric value associated with a physical unit.
///
/// Specialize descendant as necessary to handle different scalar types; for
/// example, a matrix.
///
/// The number is protected because it is here abstracted away from the unit,
/// which it needs in order to make any sense.  The number is combined with the
/// unit in a descendant class.
///
/// @tparam T  Type of number, typically `float` or `double`.
template <typename T> struct basic_number {
  T v_;             ///< Numeric value that multiplies units.
  using scalar = T; ///< Scalar type.
  basic_number() {} ///< By default, do not initialize.

  /// Initialize numeric value and exponents of units.
  /// @param v  Numeric value that multiplies units.
  constexpr basic_number(T const &vv) : v_(vv) {}
};


/// Generic template for wrapper to disambiguate scalar from dimval and
/// protected storage for numeric value associated with a physical unit.
///
/// The generic template is not used.  Specialize for each allowable scalar
/// type.
///
/// @tparam T  Type of scalar for dimval.
template <typename T> struct number {};

/// Specialization of scalar for int.
template <> struct number<int> : public basic_number<int> {
  using basic_number<int>::basic_number; ///< Inherit constructor.
  using test = int; ///< For SFINAE, type defined only in specialization.
};

/// Specialization of scalar for float.
template <> struct number<float> : public basic_number<float> {
  using basic_number<float>::basic_number; ///< Inherit constructor.
  using test = int; ///< For SFINAE, type defined only in specialization.
};

/// Specialization of scalar for double.
template <> struct number<double> : public basic_number<double> {
  using basic_number<double>::basic_number; ///< Inherit constructor.
  using test = int; ///< For SFINAE, type defined only in specialization.
};

/// Specialization of scalar for long double.
template <> struct number<long double> : public basic_number<long double> {
  using basic_number<long double>::basic_number; ///< Inherit constructor.
  using test = int; ///< For SFINAE, type defined only in specialization.
};

/// Specialization of scalar for vnix::mv::mat.
template <typename T, size_t NR, size_t NC>
struct number<mv::mat<T, NR, NC>> : public basic_number<mv::mat<T, NR, NC>> {
  /// Inherit constructor.
  using basic_number<mv::mat<T, NR, NC>>::basic_number;

  using test = int; ///< For SFINAE, type defined only in specialization.
};

/// Specialization of scalar for Eigen::Matrix.
template <typename S, int R, int C, int OPT, int MR, int MC>
struct number<Eigen::Matrix<S, R, C, OPT, MR, MC>>
    : public basic_number<Eigen::Matrix<S, R, C, OPT, MR, MC>> {
  /// Inherit constructor.
  using basic_number<Eigen::Matrix<S, R, C, OPT, MR, MC>>::basic_number;

  using test = int; ///< For SFINAE, type defined only in specialization.
};

/// Specialization of scalar for Eigen::CwiseBinaryOp.
template <typename B, typename L, typename R>
struct number<Eigen::CwiseBinaryOp<B, L, R>>
    : public basic_number<Eigen::CwiseBinaryOp<B, L, R>> {
  /// Inherit constructor.
  using basic_number<Eigen::CwiseBinaryOp<B, L, R>>::basic_number;

  using test = int; ///< For SFINAE, type defined only in specialization.
};

/// Specialization of scalar for Eigen::Product.
template <typename L, typename R, int OPT>
struct number<Eigen::Product<L, R, OPT>>
    : public basic_number<Eigen::Product<L, R, OPT>> {
  /// Inherit constructor.
  using basic_number<Eigen::Product<L, R, OPT>>::basic_number;

  using test = int; ///< For SFINAE, type defined only in specialization.
};


} // namespace units
} // namespace vnix

#endif // ndef VNIX_UNITS_NUMBER_HPP
