/* Copyright (C) 2021  Steven Petsche <stevenpetsche@gmail.com>
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
 
int format_number_user(int number,
                       char* output_buf,
                       int max_output_size) {
  sprintf(output_buf, "%i", number);
  return 0;
}
    
int parse_number_user(char* buf, int* output_number) {
  *output_number = atoi(buf);
  return 0;
}
    
int add_user(int first,
             int second,
             int* output) {
  *output = first + second;
  return 0;
}

int multiply_user(int first,
                  int second,
                  int* output) {
  *output = first * second;
  return 0;
}

