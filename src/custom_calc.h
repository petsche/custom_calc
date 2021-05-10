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

#ifndef NUMBER_TYPE
#  error "Must define target NUMBER_TYPE when compiling."
#endif

/* Keys are assigned char values for easier testing. */
typedef enum calc_input_key {
  KEY_0 = '0',
  KEY_1 = '1',
  KEY_2 = '2',
  KEY_3 = '3',
  KEY_4 = '4',
  KEY_5 = '5',
  KEY_6 = '6',
  KEY_7 = '7',
  KEY_8 = '8',
  KEY_9 = '9',
  KEY_DECIMAL = '.',
  KEY_PUSH = 'p',
  KEY_PLUS = '+'
} calc_input_key;

typedef struct calc_calc_state {
  char input_buf[IO_WIDTH];
  int input_size;
  char output_buf[IO_WIDTH];
  NUMBER_TYPE stack[MAX_STACK_SIZE];
  int stack_size;
} custom_calc_state;

int custom_calc_init(custom_calc_state* state);

int custom_calc_update(custom_calc_state* state, calc_input_key key);

int format_number_user(NUMBER_TYPE number,
                       char* output_buf,
                       int max_output_size);

int parse_number_user(char* buf, NUMBER_TYPE* output_number);

int add_user(NUMBER_TYPE first,
             NUMBER_TYPE second,
             NUMBER_TYPE* output);

#endif
