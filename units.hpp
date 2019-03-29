#ifndef VNIX_UNITS_HPP
#define VNIX_UNITS_HPP

#include "dimval.hpp"

namespace vnix {


constexpr dimval m{1.0, {1, 0, 0, 0, 0}};
constexpr dimval km = 1.0E+03 * m;
constexpr dimval cm = 1.0E-02 * m;
constexpr dimval mm = 1.0E-03 * m;
constexpr dimval μm = 1.0E-06 * m;
constexpr dimval nm = 1.0E-09 * m;
constexpr dimval pm = 1.0E-12 * m;
constexpr dimval fm = 1.0E-15 * m;

constexpr dimval kg{1.0, {0, 1, 0, 0, 0}};
constexpr dimval g = 1.0E-03 * kg;
constexpr dimval mg = 1.0E-06 * kg;
constexpr dimval μg = 1.0E-09 * kg;
constexpr dimval ng = 1.0E-12 * kg;
constexpr dimval pg = 1.0E-15 * kg;

constexpr dimval s{1.0, {0, 0, 1, 0, 0}};
constexpr dimval ms = 1.0E-03 * s;
constexpr dimval μs = 1.0E-06 * s;

constexpr dimval C{1.0, {0, 0, 0, 1, 0}};

constexpr dimval K{1.0, {0, 0, 0, 0, 1}};
constexpr dimval mK = 1.0E-03 * K;

constexpr dimval N = kg * m / s / s;

constexpr dimval J = N * m;
constexpr dimval mJ = 1.0E-03 * J;

constexpr dimval A = C / s;
constexpr dimval mA = 1.0E-03 * A;


} // namespace vnix

#endif // ndef VNIX_UNITS_HPP
