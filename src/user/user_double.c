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
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "custom_calc.h"
 
/* Only tested for CALC_IO_WIDTH = 16 */
custom_calc_status
format_number_user(double number,
                   char* output_buf,
                   char max_output_size) {
  int digits = max_output_size;
  if (number < 0.0) {
    output_buf[0] = '-';
    ++output_buf;
    --digits;
    number = -number;
  }
  if (isnan(number)) {
    output_buf[0] = 'N';
    output_buf[1] = 'a';
    output_buf[2] = 'N';
    output_buf[3] = 0;
    return CALC_STATUS_SUCCESS;
  }
  if (isinf(number)) {
    output_buf[0] = 'I';
    output_buf[1] = 'n';
    output_buf[2] = 'f';
    output_buf[3] = 0;
    return CALC_STATUS_SUCCESS;
  }
  /* Find magnitude, rounding up */
  sprintf(output_buf, "%.0E", number);
  int magnitude = atoi(output_buf + 2);
  char trailing_zeros = 0;
  if (number >= 1.0) {
    /* Check for exact int that can fit */
    long long int_cast = number;
    double int_cast_double = int_cast;
    if (magnitude + 2 <= digits && number == int_cast_double) {
        sprintf(output_buf, "%lld", int_cast);
        trailing_zeros = 0;
    } else {
      /* Heuristic that 6 decimals is enough */
      if (magnitude + 7 <= digits) {
         sprintf(output_buf, "%.6f", number);
         trailing_zeros = 1;
      } else {
        sprintf(output_buf, "%.6E", number);
      }
    }
  } else if (number >= 0.001) {
    /* Heuristic about max 0s after . for easy reading */
    sprintf(output_buf, "%.*f", digits - 2, number);
    trailing_zeros = 1;
  } else if (number == 0.0) {
    output_buf[0] = '0';
    output_buf[1] = 0;
  } else {
    sprintf(output_buf, "%.6E", number);
  }
  if (trailing_zeros) {
    char* decimal = output_buf;
    char* end_buf = output_buf + digits;
    while (*decimal != '.' && *decimal != 0 && decimal <= end_buf) {
      ++decimal;
    }
    if (*decimal != 0 && decimal != end_buf) {
      char* last_digit = decimal;
      while (*(last_digit + 1) != 0) {
        ++last_digit;
      }
      while (*last_digit == '0') {
        *last_digit = 0;
        --last_digit;
      }
      if (*last_digit == '.') {
        *last_digit = 0;
        --last_digit;
      }
    }
  }
  return CALC_STATUS_SUCCESS;
}
    
custom_calc_status
parse_number_user(char* buf,
                  double* output_number) {
  *output_number = atof(buf);
  return CALC_STATUS_SUCCESS;
}
    
custom_calc_status
operator_bifunction_user(custom_calc_key op,
                         double left,
                         double right,
                         double* output) {
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
operator_function_user(custom_calc_key op,
                       double input,
                       double* output) {
  switch (op) {
    case CALC_KEY_FLIP_SIGN:
      *output = -input;
      return CALC_STATUS_SUCCESS;
    default:
      return CALC_STATUS_UNSUPPORTED_OPERATION;
  }
}

