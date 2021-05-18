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
format_number_user(double number,
                   char* output_buf,
                   char max_output_size) {
  sprintf(output_buf, "%.3f", number);
  return CALC_STATUS_SUCCESS;
}
    
custom_calc_status
parse_number_user(char* buf,
                  double* output_number) {
  *output_number = atof(buf);
  return CALC_STATUS_SUCCESS;
}
    
custom_calc_status
add_user(double left,
         double right,
         double* output) {
  *output = left + right;
  return CALC_STATUS_SUCCESS;
}

custom_calc_status
subtract_user(double left,
              double right,
              double* output) {
  *output = left - right;
  return CALC_STATUS_SUCCESS;
}

custom_calc_status
multiply_user(double left,
              double right,
              double* output) {
  *output = left * right;
  return CALC_STATUS_SUCCESS;
}

custom_calc_status
divide_user(double left,
            double right,
            double* output) {
  *output = left / right;
  return CALC_STATUS_SUCCESS;
}

custom_calc_status
flip_sign_user(double input,
               double* output) {
  *output = -input;
  return CALC_STATUS_SUCCESS;
}
