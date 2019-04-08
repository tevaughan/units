#!/usr/bin/env perl

# Copyright 2016-2017  Thomas E. Vaughan
# Distributable under the terms of the GNU LGPL, Version 3 or later.

use strict;
use warnings;

open(my $input, '<', "units.txt");
open(my $out_h, '>', "units.hpp");
open(my $out_c, '>', "units.cpp");

print $out_h <<'EOF1';

// This file was generated by the 'units' executable.

/// \file   units.hpp
/// \brief  Automatically generated header file for units.

#ifndef NUMERIC_UNITS_HPP
#define NUMERIC_UNITS_HPP

#include "dimensions.hpp"

namespace num {
/// Namespace for physical units.
namespace u {
}
EOF1

print $out_c <<'EOF2';

// This file was generated by the 'units' executable.

/// \file   units.cpp
/// \brief  Automatically generated implementation file for units.

#include "units.hpp"

using namespace num::u;

EOF2

while(<$input>) {
   s/#.*$//;         # Remove comment.
   next if /^\s*$/;  # Skip blank line.
   my($pnm, $snm, $dim, $cnv) = split;
   print $out_h <<"EOF3";

namespace u {
/// Machine-generated structure providing public constructor, which takes a
/// double-precision number of $pnm.
struct $pnm : public $dim {
   /// Construct a dimensioned quantity from a double-precision number of
   /// $pnm.  By default, construct the zero value.
   $pnm(double v = 0.0) : $dim(v * $cnv) {}
   /// Allow default copying.
   $pnm($pnm const& dv) = default;
   /// Write representation to output stream.
   friend std::ostream& operator<<(std::ostream& os, $pnm const& dv)
   {
      /// Factor that converts to the number of $pnm from the number of the
      /// equivalent MKS unit.
      static double constexpr c = 1.0 / $cnv;
      return os << "[" << c * dv.v_ << " $snm]";
   }
};

/// Declaration of symbol $snm representing a unit of $dim.
extern $pnm const $snm;
}
EOF3
   print $out_c "$pnm const num::u::$snm(1.0);\n";
}

print $out_h <<'EOF4';
}

#endif // ndef NUMERIC_UNITS_HPP

EOF4

print $out_c "\n";

