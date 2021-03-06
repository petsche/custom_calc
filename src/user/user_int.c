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
#include "custom_calc.h"
 
custom_calc_status
format_number_user(int number,
                   char* output_buf,
                   char max_output_size) {
  sprintf(output_buf, "%i", number);
  return CALC_STATUS_SUCCESS;
}
    
custom_calc_status
parse_number_user(char* buf,
                  int* output_number) {
  *output_number = atoi(buf);
  return CALC_STATUS_SUCCESS;
}
    
custom_calc_status
binary_operator_user(custom_calc_key op,
                     int left,
                     int right,
                     int* output) {
  switch (op) {
    case CALC_KEY_ADD:
      *output = left + right;
      return CALC_STATUS_SUCCESS;
    case CALC_KEY_SUBTRACT:
      *output = left - right;
      return CALC_STATUS_SUCCESS;
    case CALC_KEY_MULTIPLY:
      *output = left * right;
      return CALC_STATUS_SUCCESS;
    case CALC_KEY_DIVIDE:
      *output = left / right;
      return CALC_STATUS_SUCCESS;
    default:
      return CALC_STATUS_UNSUPPORTED_OPERATION;
  }
}

custom_calc_status
unary_operator_user(custom_calc_key op,
                    int input,
                    int* output) {
  switch (op) {
    case CALC_KEY_FLIP_SIGN:
      *output = -input;
      return CALC_STATUS_SUCCESS;
    default:
      return CALC_STATUS_UNSUPPORTED_OPERATION;
  }
}

