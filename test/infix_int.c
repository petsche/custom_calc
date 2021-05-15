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
#include "custom_calc.h"

void main(void) { 
  printf("Custom Calculator!\n");
  custom_calc_state state;
  custom_calc_init(&state, CALC_MODE_INFIX);
  custom_calc_status ret_code = 0;
  char buffer[100];
  while (1) {
    char index = 0;
    scanf("%s", buffer);
    while (buffer[index] != 0) {
      custom_calc_key next_key = buffer[index];
      ret_code = custom_calc_update(&state, next_key);
      if (ret_code != CALC_STATUS_SUCCESS) {
        printf("Encountered error status code [%i]\n", ret_code);
      }
      ++index;
    }
    printf("%s\n", state.output_buf);
  }
}

