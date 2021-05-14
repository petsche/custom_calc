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

void test_case(custom_calc_mode mode,
               char* test_input,
               char* final_output) { 
  printf("Running test for input [%s]\n", test_input);
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
/* push = p, multiply = M, divide = d */
  test_case(CALC_MODE_RPN, "12p34p+", "46");
  test_case(CALC_MODE_RPN, "123p456+7890+", "8469");
  test_case(CALC_MODE_RPN, "5p6pM5+", "35");
  test_case(CALC_MODE_INFIX, "3+4d4M2-9=", "-4");
  test_case(CALC_MODE_INFIX, "1+5M2+4=", "15");
}

