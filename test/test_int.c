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

void assert_success(int ret_code) {
  if (ret_code != CALC_STATUS_SUCCESS) {
    printf("Test failed.\nReturn code [%i]\n");
    exit(EXIT_FAILURE);
  }
}

void assert_equal(char* expected, char* test_value) {
  if (strcmp(expected, test_value) != 0) {
    printf("Test failed.\nExpected [%s]\nActual [%s]\n",
           expected,
           test_value);
    exit(EXIT_FAILURE);
  }
}

void test_case(char* test_name,
               custom_calc_mode mode,
               char* test_input,
               char* final_output) { 
  printf("Running test '%s'\nInput [%s]\n", test_name, test_input);
  custom_calc_state state;
  custom_calc_init(&state, mode);

  char index = 0;
  custom_calc_status ret_code = 0;
  while (test_input[index] != 0) {
    custom_calc_key next_key = test_input[index];
    ret_code = custom_calc_update(&state, next_key);
    assert_success(ret_code);
    printf("%s\n", state.output_buf);
    ++index;
  }
  assert_equal(final_output, state.output_buf);

  printf("Test passed.\n");
}

int main(void) {

  test_case("When init, display zero",
            CALC_MODE_INFIX, "", "0");
  test_case("When greater, no zero",
            CALC_MODE_INFIX, "1", "1");
  test_case("When decimal, keep zero",
            CALC_MODE_INFIX, ".1", "0.1");
  test_case("When clear entry, display zero",
            CALC_MODE_INFIX, "1+2d", "0");
  test_case("When extra input, ignore",
            CALC_MODE_RPN, "1234567890", "12345678");
  test_case("When backspace empty input, ignore",
            CALC_MODE_RPN, "123bbbb4", "4");
  test_case("When clear entry, only delete current input",
            CALC_MODE_RPN, "12p34d56+", "68");
  test_case("When clear all, delete whole stack",
            CALC_MODE_RPN, "1p2pc3p4+", "7");
  test_case("When rpn add, correct result",
            CALC_MODE_RPN, "12p34p+", "46");
  test_case("When operator, auto push",
            CALC_MODE_RPN, "123p456+7890+10-", "8459");
  test_case("When rpn multiply, correct result",
            CALC_MODE_RPN, "5p6p*5+", "35");
  test_case("When infix chain operators, correct result",
            CALC_MODE_INFIX, "1-5+4=", "0");
  test_case("When diff precedence, correct evaluation order",
            CALC_MODE_INFIX, "3+4/4*2p-9=", "-4");
  test_case("When flip sign on empty, ignore",
            CALC_MODE_INFIX, "~7+5~=", "2");
  test_case("When no infix operator or input, reset",
            CALC_MODE_INFIX, "1+3=2+4=", "6");
}

