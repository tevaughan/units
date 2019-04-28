/// @file       vnix/units.hpp
/// @brief      Definition of various units in namespace vnix::units.
/// @copyright  2019  Thomas E. Vaughan
/// @license    GPL Version 3 or later.

#ifndef VNIX_UNITS_HPP
#define VNIX_UNITS_HPP

#include <vnix/units/dimval.hpp>

namespace vnix {
namespace units {


constexpr meters<long double>    operator"" _m(long double v) { return v; }
constexpr kilograms<long double> operator"" _kg(long double v) { return v; }
constexpr seconds<long double>   operator"" _s(long double v) { return v; }
constexpr coulombs<long double>  operator"" _C(long double v) { return v; }
constexpr kelvins<long double>   operator"" _K(long double v) { return v; }

constexpr auto operator"" _km(long double v) { return v * 1000.0_m; }

constexpr auto m  = 1.0_m;  ///< Meter.
constexpr auto kg = 1.0_kg; ///< Kilogram.
constexpr auto s  = 1.0_s;  ///< Second.
constexpr auto C  = 1.0_C;  ///< Coulomb.
constexpr auto K  = 1.0_K;  ///< Kelvin.

// Lengths based on meter.
constexpr auto km     = 1.0E+03 * m; ///< Kilometer.
constexpr auto cm     = 1.0E-02 * m; ///< Centimeter.
constexpr auto mm     = 1.0E-03 * m; ///< Millimeter.
constexpr auto micron = 1.0E-06 * m; ///< Micrometer.
constexpr auto nm     = 1.0E-09 * m; ///< Nanometer.
constexpr auto pm     = 1.0E-12 * m; ///< Picometer.
constexpr auto fm     = 1.0E-15 * m; ///< Femtometer.
constexpr auto ft     = 0.3048 * m;  ///< Foot.
constexpr auto yd     = 0.9144 * m;  ///< Yard.
constexpr auto mi     = 5280 * ft;   ///< Mile.

// Masses based on kilogram.
constexpr auto g         = 1.0E-03 * kg; ///< Gram.
constexpr auto mg        = 1.0E-06 * kg; ///< Milligram.
constexpr auto microgram = 1.0E-09 * kg; ///< Microgram.
constexpr auto ng        = 1.0E-12 * kg; ///< Nanogram.
constexpr auto pg        = 1.0E-15 * kg; ///< Picogram.

// Times based on second.
constexpr auto ms       = 1.0E-03 * s; ///< Millisecond.
constexpr auto microsec = 1.0E-06 * s; ///< Microsecond.
constexpr auto ns       = 1.0E-09 * s; ///< Nanosecond.
constexpr auto ps       = 1.0E-12 * s; ///< Picosecond.
constexpr auto fs       = 1.0E-15 * s; ///< Femtosecond.

// Temperatures based on Kelvin.
constexpr auto mK = 1.0E-03 * K; ///< Millikelvin.

// Forces.
constexpr auto N   = kg * m / s / s; ///< Newton.
constexpr auto dyn = g * cm / s / s; ///< Dyne.

// Energies.
constexpr auto J   = N * m;       ///< Joule.
constexpr auto mJ  = 1.0E-03 * J; ///< Millijoule.
constexpr auto kJ  = 1.0E+03 * J; ///< Kilojoule.
constexpr auto MJ  = 1.0E+06 * J; ///< Megajoule.
constexpr auto erg = dyn * cm;    ///< Erg.

// Currents.
constexpr auto A  = C / s;       ///< Amp.
constexpr auto mA = 1.0E-03 * A; ///< Milliamp.


// Some useful statdim types (other than length, time, etc., already defined).
using force    = decltype(N);          ///< statdim for force.
using energy   = decltype(J);          ///< statdim for energy.
using current  = decltype(A);          ///< statdim for electric current.
using speed    = decltype(m / s);      ///< statdim for speed.
using momentum = decltype(kg * m / s); ///< statdim for momentum.
using area     = decltype(m * m);      ///< statdim for area.
using volume   = decltype(m * m * m);  ///< statdim for volume.
using pressure = decltype(N / m / m);  ///< statdim for pressure.


} // namespace units
} // namespace vnix

#endif // ndef VNIX_UNITS_HPP
