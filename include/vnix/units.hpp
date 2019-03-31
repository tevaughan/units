/// @file       units.hpp
/// @brief      Definition of various units in namespace vnix::units.
/// @copyright  2019 Thomas E. Vaughan
/// @license    GPL3.

#ifndef VNIX_UNITS_HPP
#define VNIX_UNITS_HPP

#include <vnix/units/dimval.hpp>

namespace vnix {
namespace units {


constexpr dimval m{1.0, {1, 0, 0, 0, 0}};  ///< Meter.
constexpr dimval kg{1.0, {0, 1, 0, 0, 0}}; ///< Kilogram.
constexpr dimval s{1.0, {0, 0, 1, 0, 0}};  ///< Second.
constexpr dimval C{1.0, {0, 0, 0, 1, 0}};  ///< Coulomb.
constexpr dimval K{1.0, {0, 0, 0, 0, 1}};  ///< Kelvin.

// Lengths based on meter.
constexpr dimval km = 1.0E+03 * m; ///< Kilometer.
constexpr dimval cm = 1.0E-02 * m; ///< Centimeter.
constexpr dimval mm = 1.0E-03 * m; ///< Millimeter.
constexpr dimval μm = 1.0E-06 * m; ///< Micrometer.
constexpr dimval nm = 1.0E-09 * m; ///< Nanometer.
constexpr dimval pm = 1.0E-12 * m; ///< Picometer.
constexpr dimval fm = 1.0E-15 * m; ///< Femtometer.
constexpr dimval ft = 0.3048 * m;  ///< Foot.
constexpr dimval yd = 0.9144 * m;  ///< Yard.
constexpr dimval mi = 5280 * ft;   ///< Mile.

// Masses based on kilogram.
constexpr dimval g  = 1.0E-03 * kg; ///< Gram.
constexpr dimval mg = 1.0E-06 * kg; ///< Milligram.
constexpr dimval μg = 1.0E-09 * kg; ///< Microgram.
constexpr dimval ng = 1.0E-12 * kg; ///< Nanogram.
constexpr dimval pg = 1.0E-15 * kg; ///< Picogram.

// Times based on second.
constexpr dimval ms = 1.0E-03 * s; ///< Millisecond.
constexpr dimval μs = 1.0E-06 * s; ///< Microsecond.
constexpr dimval ns = 1.0E-09 * s; ///< Nanosecond.
constexpr dimval ps = 1.0E-12 * s; ///< Picosecond.
constexpr dimval fs = 1.0E-15 * s; ///< Femtosecond.

// Temperatures based on Kelvin.
constexpr dimval mK = 1.0E-03 * K; ///< Millikelvin.

// Forces.
constexpr dimval N   = kg * m / s / s; ///< Newton.
constexpr dimval dyn = g * cm / s / s; ///< Dyne.

// Energies.
constexpr dimval J   = N * m;       ///< Joule.
constexpr dimval mJ  = 1.0E-03 * J; ///< Millijoule.
constexpr dimval kJ  = 1.0E+03 * J; ///< Kilojoule.
constexpr dimval MJ  = 1.0E+06 * J; ///< Megajoule.
constexpr dimval erg = dyn * cm;    ///< Erg.

// Currents.
constexpr dimval A  = C / s;       ///< Amp.
constexpr dimval mA = 1.0E-03 * A; ///< Milliamp.


} // namespace units
} // namespace vnix

#endif // ndef VNIX_UNITS_HPP
