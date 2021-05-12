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
#ifndef CUSTOM_CALC_H
#define CUSTOM_CALC_H

#ifndef IO_WIDTH
#define IO_WIDTH 16
#endif

#ifndef MAX_STACK_SIZE
#define MAX_STACK_SIZE 10
#endif

#ifndef CALC_NUMBER_TYPE
#  error "Must define target CALC_NUMBER_TYPE when compiling."
#endif

/* Keys are assigned char values for easier testing. */
typedef enum custom_calc_key {
  CALC_KEY_0 = '0',
  CALC_KEY_1 = '1',
  CALC_KEY_2 = '2',
  CALC_KEY_3 = '3',
  CALC_KEY_4 = '4',
  CALC_KEY_5 = '5',
  CALC_KEY_6 = '6',
  CALC_KEY_7 = '7',
  CALC_KEY_8 = '8',
  CALC_KEY_9 = '9',
  CALC_KEY_DECIMAL = '.',
  CALC_KEY_FLIP_SIGN = '~',
  CALC_KEY_PUSH = 'p',
  CALC_KEY_PLUS = '+', /* 43 */
  CALC_KEY_MINUS = '-', /* 45 */
  CALC_KEY_MULTIPLY = 'M', /* 77 */
  CALC_KEY_DIVIDE = 'd' /* 100 */
} custom_calc_key;

typedef struct custom_calc_state {
  char input_buf[IO_WIDTH + 1];
  int input_size;
  char output_buf[IO_WIDTH + 1];
  CALC_NUMBER_TYPE stack[MAX_STACK_SIZE];
  int stack_size;
} custom_calc_state;

int custom_calc_init(custom_calc_state* state);

int custom_calc_update(custom_calc_state* state, custom_calc_key key);

int format_number_user(CALC_NUMBER_TYPE number,
                       char* output_buf,
                       int max_output_size);

int parse_number_user(char* buf, CALC_NUMBER_TYPE* output_number);

int add_user(CALC_NUMBER_TYPE first,
             CALC_NUMBER_TYPE second,
             CALC_NUMBER_TYPE* output);

int multiply_user(CALC_NUMBER_TYPE first,
                  CALC_NUMBER_TYPE second,
                  CALC_NUMBER_TYPE* output);

#endif
