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
  printf("***Running double test...");

  test_case("When init, display zero",
            CALC_MODE_INFIX, "1p", "1.000");
  test_case("When rpn add, correct result",
            CALC_MODE_RPN, "1p2p+", "3.000");
  test_case("When rpn subtract, correct result",
            CALC_MODE_RPN, "3p1p-", "2.000");
  test_case("When rpn multiply, correct result",
            CALC_MODE_RPN, "3p2p*", "6.000");
  test_case("When rpn divide, correct result",
            CALC_MODE_RPN, "8p2p/", "4.000");
  test_case("When rpn flip sign, correct result",
            CALC_MODE_INFIX, "1~", "-1.000");
}

