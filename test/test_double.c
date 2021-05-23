/* Copyright (C) 2021 Steven Petsche <stevenpetsche@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "custom_calc.h"
#include "test_util.h"

int main(void) {
  printf("********************Running double test********************\n");

  test_case("When max exact int for size, no decimal",
            CALC_MODE_INFIX, "949999999999999p", "949999999999999");
  test_case("When over max int for size, E notation",
            CALC_MODE_INFIX, "950000000000000p", "9.500000E+14");
  test_case("When over 0.001, decimal notation",
            CALC_MODE_INFIX, "0.0011p", "0.0011");
  test_case("When room for 6 decimal places, decimal notation",
            CALC_MODE_INFIX, "1.5p", "1.5");
  test_case("When no room for 6 decimal places, E notation",
            CALC_MODE_INFIX, "12345678901.2p", "1.234568E+10");
  test_case("When too many zeros after decimal, E notation",
            CALC_MODE_INFIX, "0.0002p", "2.000000E-04");
  test_case("When negative, show sign",
            CALC_MODE_INFIX, "0.002p~", "-0.002");
  test_case("When trailing zeros, trim",
            CALC_MODE_INFIX, "0.002000p", "0.002");
  test_case("When no decimal, no trim",
            CALC_MODE_INFIX, "2000p", "2000");
  test_case("When just decimal, trim",
            CALC_MODE_INFIX, "2000.p", "2000");
  test_case("When init, display zero",
            CALC_MODE_INFIX, "1p", "1");
  test_case("When rpn add, correct result",
            CALC_MODE_RPN, "1p2p+", "3");
  test_case("When rpn subtract, correct result",
            CALC_MODE_RPN, "3p1p-", "2");
  test_case("When rpn multiply, correct result",
            CALC_MODE_RPN, "3p2p*", "6");
  test_case("When rpn divide, correct result",
            CALC_MODE_RPN, "8p2p/", "4");
  test_case("When rpn flip sign, correct result",
            CALC_MODE_INFIX, "1~", "-1");
}

