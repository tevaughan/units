
#include <array>                 // for array
#include <eigen3/Eigen/Geometry> // for AngleAxis, Matrix, etc.
#include <iostream>              // for cout, etc.
#include <type_traits>           // for remove_const
#include <utility>               // for index_sequence
#include <vnix/units.hpp>

using std::array;
using std::cerr;
using std::cout;
using std::endl;
using std::index_sequence;
using std::make_index_sequence;
using std::ostream;

namespace vnix {
namespace mv {


/// Convert element i of array a from type OT to type T.
template <typename T, typename OT, size_t N>
constexpr T cnv_el(array<OT, N> const &a, size_t i) {
  return a[i];
}

/// Convert array of elements of type OT to array of elements of type T.
template <typename T, typename OT, size_t... i>
constexpr auto _cnv(array<OT, sizeof...(i)> const &a, index_sequence<i...>) {
  return array<T, sizeof...(i)>{{cnv_el<T>(a, i)...}};
}

/// Convert array of elements of type OT to array of elements of type T.
template <typename T, typename OT, size_t N>
constexpr auto cnv_ar(array<OT, N> const &a) {
  return _cnv<T>(a, make_index_sequence<N>{});
}


/// Reference to column or row in matrix.
template <typename T, size_t S, size_t N> class mref {
  T *beg_; ///< Pointer to first element.

public:
  constexpr mref(T *b) : beg_(b) {}                ///< Initialize aggregate.
  constexpr static size_t size() { return N; }     ///< Number of elements.
  constexpr T *           begin() { return beg_; } ///< First element.

  /// Pointer to element that is S elements past last element identified by
  /// mref.
  constexpr T *end() { return beg_ + S * N; }

  /// Assign from list.
  constexpr mref &operator=(std::initializer_list<T> list) {
    auto i  = list.begin();
    auto j  = begin();
    auto ie = list.end();
    auto je = end();
    while (i != ie && j != je) {
      *j++ = *i;
      i += S;
    }
    return *this;
  }

  /// Distance in memory between successive elements.
  constexpr static size_t stride() { return S; }

  /// Access element at offset off.
  constexpr T &operator()(size_t off) const { return beg_[S * off]; }
};

/// Dot-product of two mrefs.
template <typename T1, typename T2, size_t S1, size_t S2, size_t N>
constexpr auto dot(mref<T1, S1, N> const &mr1, mref<T2, S2, N> const &mr2) {
  auto sum = 0 * mr1(0) * mr2(0);
  for (size_t i = 0; i < N; ++i) { sum += mr1(i) * mr2(i); }
  return sum;
}


/// Model of a matrix of quantities.
template <typename T, size_t NR, size_t NC> struct mat {
  array<T, NR * NC> a; ///< Array in which quantities are stored.

  /// Allow access to every other type of matrix.
  template <typename OT, size_t ONR, size_t ONC> friend struct mat;

  mat() {}

  /// Initialize from list.
  template <typename... X> constexpr mat(X... xs) : a({T(xs)...}) {}

  /// Copy from same-size matrix of other element-type OT.
  template <typename OT>
  constexpr mat(mat<OT, NR, NC> const &m) : a(cnv_ar<T>(m.a)) {}

  /// Reference to immutable column.
  constexpr auto col(size_t off) const {
    return mref<T const, NC, NR>(a.data() + off);
  }

  /// Reference to mutable column.
  constexpr auto col(size_t off) {
    using RT = typename std::remove_const<T>::type;
    return mref<RT, NC, NR>(a.data() + off);
  }

  /// Reference to immutable row.
  constexpr auto row(size_t off) const {
    return mref<T const, 1, NC>(a.data() + NC * off);
  }

  /// Reference to mutable row.
  constexpr auto row(size_t off) {
    using RT = typename std::remove_const<T>::type;
    return mref<RT, 1, NC>(a.data() + NC * off);
  }
};

/// Model of column of quantities.
template <typename T, size_t NR> using col = mat<T, NR, 1>;

/// Model of row of quantities.
template <typename T, size_t NC> using row = mat<T, 1, NC>;

/// Multiply two matrices.
template <typename T1, typename T2, size_t NR1, size_t NC2, size_t N>
constexpr auto operator*(mat<T1, NR1, N> const &m1,
                         mat<T2, N, NC2> const &m2) {
  using TT = decltype(dot(m1.row(0), m2.col(0)));
  using TP = typename std::remove_const<TT>::type;
  mat<TP, NR1, NC2> pr; // product
  for (size_t i = 0; i < NR1; ++i) {
    auto const m1_rowi = m1.row(i);
    auto       pr_rowi = pr.row(i);
    for (size_t j = 0; j < NC2; ++j) { pr_rowi(j) = dot(m1_rowi, m2.col(j)); }
  }
  return pr;
}

/// Multiply matrix on left by scalar.
template <typename T1, typename T2, size_t NR2, size_t NC2>
constexpr auto operator*(T1 const &s1, mat<T2, NR2, NC2> const &m2) {
  mat<decltype(s1 * m2.row(0)(0)), NR2, NC2> pr; // product
  for (size_t i = 0; i < NR2; ++i) {
    auto const m2_rowi = m2.row(i);
    auto       pr_rowi = pr.row(i);
    for (size_t j = 0; j < NC2; ++j) { pr_rowi(j) = s1 * m2_rowi(j); }
  }
  return pr;
}

/// Print matrix.
template <typename T, size_t NR, size_t NC>
ostream &operator<<(ostream &os, mat<T, NR, NC> const &m) {
  for (size_t i = 0; i < NR; ++i) {
    auto const rowi = m.row(i);
    os << endl;
    for (size_t j = 0; j < NC; ++j) { os << rowi(j) << "  "; }
  }
  return os;
}


} // namespace mv
} // namespace vnix

int main() {
  try {
    using namespace vnix::mv;
    mat<float, 3, 3> m1;
    m1.row(0) = {1, 2, 3};
    m1.row(1) = {4, 5, 6};
    m1.row(2) = {7, 8, 9};
    col<double, 3> v({0.3, 0.2, 0.1});
    auto           f = vnix::units::newtons(m1);
    auto           d = vnix::units::kilometers(v);
    cout << (f * d) << endl;
  } catch (char const *e) { cerr << e << endl; }

  cout << endl;

  try {
    using namespace vnix::mv;
    using namespace vnix::units::flt;
    force            foo;
    mat<force, 3, 3> f2;
    f2.row(0) = {1.0_N, 2.0_N, 3.0_N};
    f2.row(1) = {4.0_N, 5.0_N, 6.0_N};
    f2.row(2) = {7.0_N, 8.0_N, 9.0_N};
    col<length, 3> d2({0.3_km, 0.2_km, 0.1_km});
    cout << (f2 * d2) << endl;
  } catch (char const *e) { cerr << e << endl; }

  cout << endl;

  try {
    using namespace Eigen;
    using namespace vnix::units;
    using namespace vnix::units::flt;
    AngleAxisf r(M_PI / 6, Vector3f(0, 0, 1));
    auto       d = meters(Vector3f(0, 1, 0));
    auto       v = d / (2.5_s);
    flt::time  t = 3.0_s;
    cout << r.toRotationMatrix() * (d + v * t) << endl;
  } catch (char const *e) { cerr << e << endl; }

  return 0;
}

