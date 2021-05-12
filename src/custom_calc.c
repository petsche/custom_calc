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
#include "custom_calc.h"

int custom_calc_init(custom_calc_state* state) {
  state->input_buf[0] = 0;
  state->input_size = 0;
  state->output_buf[0] = 0;
  state->stack_size = 0;
}

void string_copy(char* dest, const char* source) {
  /* Assumes source is null-terminated */
  char index = 0;
  do {
    dest[index] = source[index];
  } while (source[index++] != 0);
}

int process_number_input(char* input_buf, int* input_size, char input) {
  if (*input_size == IO_WIDTH) {
    return -2;
  }

  input_buf[*input_size] = input;
  ++(*input_size);
  input_buf[*input_size] = 0;
  return 0;
}

int update_output_buf(custom_calc_state* state) {
  if (state->input_size > 0) {
    string_copy(state->output_buf, state->input_buf);
    return 0;
  }
  if (state->stack_size > 0) {
    return format_number_user(state->stack[state->stack_size - 1],
                              state->output_buf,
                              IO_WIDTH);
  }
  state->output_buf[0] = 0;
  return 0;
}

int push_input_buf(custom_calc_state* state) {
  if (state->stack_size == MAX_STACK_SIZE) {
    return -3;
  }

  int ret_code = parse_number_user(state->input_buf,
                                   &(state->stack[state->stack_size]));

  if (ret_code == 0) {
    state->input_buf[0] = 0;
    state->input_size = 0;
    ++(state->stack_size);
  }
  return ret_code;
}

int pop_operator(custom_calc_state* state, custom_calc_key op) {
  if (state->stack_size < 2) {
    return -4;
  }

  int ret_code = 0;
  NUMBER_TYPE op_output;
  switch (op) {
    case CALC_KEY_PLUS:
      ret_code = add_user(state->stack[state->stack_size - 2],
                          state->stack[state->stack_size - 1],
                          &op_output);
      break;
    case CALC_KEY_MULTIPLY:
      ret_code = multiply_user(state->stack[state->stack_size - 2],
                               state->stack[state->stack_size - 1],
                               &op_output);
      break;
    default:
      ret_code = -5;
  }
  if (ret_code == 0) {
      state->stack[state->stack_size - 2] = op_output;
      --(state->stack_size);
  }
    
  return ret_code;
}

int process_operator(custom_calc_state* state, custom_calc_key key) {
  int ret_code = 0;
}

int custom_calc_update(custom_calc_state* state, custom_calc_key key) {
  int ret_code = 0;
  switch (key) {
    case CALC_KEY_0:
    case CALC_KEY_1:
    case CALC_KEY_2:
    case CALC_KEY_3:
    case CALC_KEY_4:
    case CALC_KEY_5:
    case CALC_KEY_6:
    case CALC_KEY_7:
    case CALC_KEY_8:
    case CALC_KEY_9:
    case CALC_KEY_DECIMAL:
      ret_code = process_number_input(state->input_buf,
                                      &(state->input_size),
                                      (char)key);
      break;
    case CALC_KEY_PUSH:
      ret_code = push_input_buf(state);
      break;
    case CALC_KEY_PLUS:
    case CALC_KEY_MULTIPLY:
      if (state->input_size > 0) {
        ret_code = push_input_buf(state);
      }
      if (ret_code != 0) break;
      ret_code = pop_operator(state, key);
      break;
    default:
      ret_code = -1;
  }
  if (ret_code != 0) return ret_code;

  ret_code = update_output_buf(state);
 
  return ret_code;
}

