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
#ifndef CUSTOM_CALC_H
#define CUSTOM_CALC_H

#ifndef CALC_IO_WIDTH
#define CALC_IO_WIDTH 8
#endif

#ifndef CALC_MAX_STACK_SIZE
#define CALC_MAX_STACK_SIZE 10
#endif

#ifndef CALC_NUMBER_TYPE
#  error "Must define target CALC_NUMBER_TYPE when compiling."
#endif

/* Keys are assigned char values for easier testing. */
typedef enum custom_calc_key {
  CALC_KEY_UNKNOWN = 0,
  CALC_KEY_CLEAR_ALL = 'c',
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
  CALC_KEY_BACKSPACE = 'b',
  CALC_KEY_CLEAR_ENTRY = 'd',
  CALC_KEY_FLIP_SIGN = '~',
  CALC_KEY_PUSH = 'p',
  CALC_KEY_EQUALS = '=',
  CALC_KEY_ADD = '+',
  CALC_KEY_SUBTRACT = '-',
  CALC_KEY_MULTIPLY = '*',
  CALC_KEY_DIVIDE = '/'
} custom_calc_key;

typedef enum custom_calc_mode {
  CALC_MODE_RPN,
  CALC_MODE_INFIX
} custom_calc_mode;

typedef enum custom_calc_status {
  CALC_STATUS_SUCCESS,
  CALC_STATUS_UNSUPPORTED_KEY,
  CALC_STATUS_UNSUPPORTED_OPERATION,
  CALC_STATUS_UNSUPPORTED_MODE,
  CALC_STATUS_INVALID_OPERATION,
  CALC_STATUS_STACK_OVERFLOW,
  CALC_STATUS_BUG
} custom_calc_status;

typedef struct custom_calc_state {
  char input_buf[CALC_IO_WIDTH + 1];
  char input_size;
  char output_buf[CALC_IO_WIDTH + 1];
  CALC_NUMBER_TYPE rpn_stack[CALC_MAX_STACK_SIZE];
  char rpn_stack_size;
  custom_calc_mode mode;
  custom_calc_key operator_stack[CALC_MAX_STACK_SIZE];
  char operator_stack_size;
} custom_calc_state;

custom_calc_status
custom_calc_init(custom_calc_state* state,
                 custom_calc_mode mode);

custom_calc_status
custom_calc_update(custom_calc_state* state, custom_calc_key key);

/* Functions to be implemented for user's target architecture */
custom_calc_status
format_number_user(CALC_NUMBER_TYPE number,
                   char* output_buf,
                   char max_output_size);

custom_calc_status
parse_number_user(char* buf,
                  CALC_NUMBER_TYPE* output_number);

custom_calc_status
add_user(CALC_NUMBER_TYPE left,
         CALC_NUMBER_TYPE right,
         CALC_NUMBER_TYPE* output);

custom_calc_status
subtract_user(CALC_NUMBER_TYPE left,
              CALC_NUMBER_TYPE right,
              CALC_NUMBER_TYPE* output);

custom_calc_status
multiply_user(CALC_NUMBER_TYPE left,
              CALC_NUMBER_TYPE right,
              CALC_NUMBER_TYPE* output);

custom_calc_status
divide_user(CALC_NUMBER_TYPE left,
            CALC_NUMBER_TYPE right,
            CALC_NUMBER_TYPE* output);

custom_calc_status
flip_sign_user(CALC_NUMBER_TYPE input,
               CALC_NUMBER_TYPE* output);

#endif
