#include "../vnix/gcd.hpp"
#include "catch.hpp"

using vnix::gcd;


TEST_CASE("GCD of any x and itself is x.", "[gcd]") {
  REQUIRE(gcd(0, 0) == 0);
  REQUIRE(gcd(1, 1) == 1);
  REQUIRE(gcd(2, 2) == 2);
  REQUIRE(gcd(3, 3) == 3);
  REQUIRE(gcd(4, 4) == 4);
  REQUIRE(gcd(5, 5) == 5);
  REQUIRE(gcd(6, 6) == 6);
  REQUIRE(gcd(7, 7) == 7);
  REQUIRE(gcd(8, 8) == 8);
  REQUIRE(gcd(9, 9) == 9);
}


TEST_CASE("GCD of 0 and any x is x.", "[gcd]") {
  REQUIRE(gcd(0, 1) == 1);
  REQUIRE(gcd(0, 2) == 2);
  REQUIRE(gcd(0, 3) == 3);
  REQUIRE(gcd(0, 4) == 4);
  REQUIRE(gcd(0, 5) == 5);
  REQUIRE(gcd(0, 6) == 6);
  REQUIRE(gcd(0, 7) == 7);
  REQUIRE(gcd(0, 8) == 8);
  REQUIRE(gcd(0, 9) == 9);

  REQUIRE(gcd(1, 0) == 1);
  REQUIRE(gcd(2, 0) == 2);
  REQUIRE(gcd(3, 0) == 3);
  REQUIRE(gcd(4, 0) == 4);
  REQUIRE(gcd(5, 0) == 5);
  REQUIRE(gcd(6, 0) == 6);
  REQUIRE(gcd(7, 0) == 7);
  REQUIRE(gcd(8, 0) == 8);
  REQUIRE(gcd(9, 0) == 9);
}


TEST_CASE("GCD of 1 and any x is 1.", "[gcd]") {
  REQUIRE(gcd(1, 2) == 1);
  REQUIRE(gcd(1, 3) == 1);
  REQUIRE(gcd(1, 4) == 1);
  REQUIRE(gcd(1, 5) == 1);
  REQUIRE(gcd(1, 6) == 1);
  REQUIRE(gcd(1, 7) == 1);
  REQUIRE(gcd(1, 8) == 1);
  REQUIRE(gcd(1, 9) == 1);

  REQUIRE(gcd(2, 1) == 1);
  REQUIRE(gcd(3, 1) == 1);
  REQUIRE(gcd(4, 1) == 1);
  REQUIRE(gcd(5, 1) == 1);
  REQUIRE(gcd(6, 1) == 1);
  REQUIRE(gcd(7, 1) == 1);
  REQUIRE(gcd(8, 1) == 1);
  REQUIRE(gcd(9, 1) == 1);
}


TEST_CASE("GCD of any relative primes is 1.", "[gcd]") {
  REQUIRE(gcd(2, 3) == 1);
  REQUIRE(gcd(2, 5) == 1);
  REQUIRE(gcd(2, 7) == 1);
  REQUIRE(gcd(3, 2) == 1);
  REQUIRE(gcd(3, 4) == 1);
  REQUIRE(gcd(3, 5) == 1);
  REQUIRE(gcd(3, 7) == 1);
  REQUIRE(gcd(3, 8) == 1);
  REQUIRE(gcd(4, 3) == 1);
  REQUIRE(gcd(4, 5) == 1);
  REQUIRE(gcd(4, 7) == 1);
  REQUIRE(gcd(4, 9) == 1);
  REQUIRE(gcd(5, 2) == 1);
  REQUIRE(gcd(5, 3) == 1);
  REQUIRE(gcd(5, 4) == 1);
  REQUIRE(gcd(5, 7) == 1);
  REQUIRE(gcd(5, 8) == 1);
  REQUIRE(gcd(5, 9) == 1);
  REQUIRE(gcd(6, 5) == 1);
  REQUIRE(gcd(6, 7) == 1);
  REQUIRE(gcd(7, 2) == 1);
  REQUIRE(gcd(7, 3) == 1);
  REQUIRE(gcd(7, 4) == 1);
  REQUIRE(gcd(7, 5) == 1);
  REQUIRE(gcd(7, 6) == 1);
  REQUIRE(gcd(7, 8) == 1);
  REQUIRE(gcd(7, 9) == 1);
  REQUIRE(gcd(8, 3) == 1);
  REQUIRE(gcd(8, 5) == 1);
  REQUIRE(gcd(8, 7) == 1);
  REQUIRE(gcd(8, 9) == 1);
  REQUIRE(gcd(9, 2) == 1);
  REQUIRE(gcd(9, 4) == 1);
  REQUIRE(gcd(9, 5) == 1);
  REQUIRE(gcd(9, 7) == 1);
  REQUIRE(gcd(9, 8) == 1);
}


TEST_CASE("GCD of multiple is factor.", "[gcd]") {
  REQUIRE(gcd(2, 4) == 2);
  REQUIRE(gcd(2, 6) == 2);
  REQUIRE(gcd(2, 8) == 2);
  REQUIRE(gcd(3, 6) == 3);
  REQUIRE(gcd(3, 9) == 3);
  REQUIRE(gcd(4, 8) == 4);

  REQUIRE(gcd(4, 2) == 2);
  REQUIRE(gcd(6, 2) == 2);
  REQUIRE(gcd(8, 2) == 2);
  REQUIRE(gcd(6, 3) == 3);
  REQUIRE(gcd(9, 3) == 3);
  REQUIRE(gcd(8, 4) == 4);
}


TEST_CASE("Otherwise, GCD works.", "[gcd]") {
  REQUIRE(gcd(4, 6) == 2);
  REQUIRE(gcd(6, 4) == 2);
  REQUIRE(gcd(6, 9) == 3);
  REQUIRE(gcd(9, 6) == 3);
}
