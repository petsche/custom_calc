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
#include "custom_calc.h"

void
reset_input_buf(custom_calc_state* state) {
  state->input_buf[0] = '0';
  state->input_buf[1] = 0;
  state->input_size = 1;
}

void string_copy(char* dest, const char* source) {
  /* Assumes source is null-terminated */
  char index = 0;
  do {
    dest[index] = source[index];
  } while (source[index++] != 0);
}

void
custom_calc_init(custom_calc_state* state,
                 custom_calc_mode mode) {
  reset_input_buf(state);
  string_copy(state->output_buf, state->input_buf);
  state->rpn_stack_size = 0;
  state->mode = mode;
  state->operator_stack_size = 0;
}

custom_calc_status
process_number_input(custom_calc_state* state,
                     custom_calc_key key) {
  if (key == CALC_KEY_CLEAR_ENTRY) {
    reset_input_buf(state);
    return CALC_STATUS_SUCCESS;
  } else if (key == CALC_KEY_BACKSPACE) {
    --(state->input_size);
    state->input_buf[state->input_size] = 0;
    /* Put back 0 if empty */
    if (state->input_size == 0) {
      reset_input_buf(state);
    }
    return CALC_STATUS_SUCCESS;
  } else {
    if (state->input_size == CALC_IO_WIDTH) {
      /* Ignore if at limit */
      return CALC_STATUS_SUCCESS;
    }
    if (key != CALC_KEY_DECIMAL &&
        state->input_size == 1 &&
        state->input_buf[0] == '0') {
      /* Ignore placeholder zero */
      state->input_size = 0;
    }
    state->input_buf[state->input_size] = (char)key;
    ++(state->input_size);
    state->input_buf[state->input_size] = 0;
    return CALC_STATUS_SUCCESS;
  }
}

custom_calc_status
update_output_buf(custom_calc_state* state) {
  if (state->input_size > 0) {
    string_copy(state->output_buf, state->input_buf);
    return 0;
  }
  if (state->rpn_stack_size > 0) {
    return format_number_user(state->rpn_stack[state->rpn_stack_size - 1],
                              state->output_buf,
                              CALC_IO_WIDTH);
  }
  state->output_buf[0] = 0;
  return 0;
}

custom_calc_status
push_input_buf(custom_calc_state* state) {
  if (state->rpn_stack_size == CALC_MAX_STACK_SIZE) {
    return -3;
  }

  custom_calc_status ret_code = parse_number_user(state->input_buf,
                                   &(state->rpn_stack[state->rpn_stack_size]));

  if (ret_code == 0) {
    state->input_buf[0] = 0;
    state->input_size = 0;
    ++(state->rpn_stack_size);
  }
  return ret_code;
}

custom_calc_status
apply_operator_rpn(custom_calc_state* state, custom_calc_key op) {
  if (state->rpn_stack_size < 2) {
    return -4;
  }

  custom_calc_status ret_code = 0;
  CALC_NUMBER_TYPE left = state->rpn_stack[state->rpn_stack_size - 2];
  CALC_NUMBER_TYPE right = state->rpn_stack[state->rpn_stack_size - 1];
  CALC_NUMBER_TYPE op_output;
  switch (op) {
    case CALC_KEY_ADD:
      ret_code = add_user(left, right, &op_output);
      break;
    case CALC_KEY_SUBTRACT:
      ret_code = subtract_user(left, right, &op_output);
      break;
    case CALC_KEY_MULTIPLY:
      ret_code = multiply_user(left, right, &op_output);
      break;
    case CALC_KEY_DIVIDE:
      ret_code = divide_user(left, right, &op_output);
      break;
    default:
      ret_code = -5;
  }
  if (ret_code == 0) {
      /* Pop two values and push new value */
      state->rpn_stack[state->rpn_stack_size - 2] = op_output;
      --(state->rpn_stack_size);
  }
    
  return ret_code;
}

custom_calc_key operator_stack_top(custom_calc_state* state) {
  return state->operator_stack[state->operator_stack_size - 1];
}

custom_calc_key operator_stack_pop(custom_calc_state* state) {
  --(state->operator_stack_size);
  return state->operator_stack[state->operator_stack_size];
}

char operator_precedence(custom_calc_key operator) {
  switch (operator) {
    case CALC_KEY_ADD:
    case CALC_KEY_SUBTRACT:
      return 1;
    case CALC_KEY_MULTIPLY:
    case CALC_KEY_DIVIDE:
      return 2;
  }
  return 0;
}

custom_calc_status
push_infix_operator(custom_calc_state* state, custom_calc_key key) {
  custom_calc_status ret_code = 0;
  while (state->operator_stack_size > 0 &&
         operator_precedence(operator_stack_top(state))
         >= operator_precedence(key)) {
    ret_code = apply_operator_rpn(state,
                            operator_stack_pop(state));
    if (ret_code != CALC_STATUS_SUCCESS) break;
  }
  if (state->operator_stack_size == CALC_MAX_STACK_SIZE) {
    ret_code = -8;
  }
  if (ret_code == CALC_STATUS_SUCCESS) {
    state->operator_stack[state->operator_stack_size] = key;
    ++(state->operator_stack_size);
  }
  return ret_code;
}

custom_calc_status
process_operator(custom_calc_state* state, custom_calc_key key) {
  custom_calc_status ret_code = 0;
  if (key == CALC_KEY_FLIP_SIGN) {
    if (state->rpn_stack_size > 0) {
      return flip_sign_user(state->rpn_stack[state->rpn_stack_size - 1],
                            &(state->rpn_stack[state->rpn_stack_size - 1]));
    } else {
      /* Ignore if no value */
      return CALC_STATUS_SUCCESS;
    }
  }

  switch (state->mode) {
    case CALC_MODE_RPN:
      ret_code = apply_operator_rpn(state, key);
      break;
    case CALC_MODE_INFIX:
      if (key == CALC_KEY_EQUALS) {
        while (state->operator_stack_size > 0) {
          ret_code = apply_operator_rpn(state, operator_stack_pop(state));
          if (ret_code != CALC_STATUS_SUCCESS) break;
        }
      } else {
        ret_code = push_infix_operator(state, key);
      }
      break;
    default:
      return -6;
  }
  return ret_code;
}

custom_calc_status
custom_calc_update(custom_calc_state* state, custom_calc_key key) {
  custom_calc_status ret_code = 0;
  switch (key) {
    case CALC_KEY_CLEAR_ALL:
       custom_calc_init(state, state->mode);
       return CALC_STATUS_SUCCESS;
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
      if (state->mode == CALC_MODE_INFIX &&
          state->input_size == 0 &&
          state->operator_stack_size == 0) {
        custom_calc_init(state, state->mode);
      }
      /* Fall through */
    case CALC_KEY_BACKSPACE:
    case CALC_KEY_CLEAR_ENTRY:
      ret_code = process_number_input(state, key);
      break;
    case CALC_KEY_PUSH:
      ret_code = push_input_buf(state);
      break;
    case CALC_KEY_ADD:
    case CALC_KEY_SUBTRACT:
    case CALC_KEY_MULTIPLY:
    case CALC_KEY_DIVIDE:
    case CALC_KEY_FLIP_SIGN:
    case CALC_KEY_EQUALS:
      if (state->input_size > 0) {
        ret_code = push_input_buf(state);
      }
      if (ret_code != 0) break;
      ret_code = process_operator(state, key);
      break;
    default:
      ret_code = -1;
  }
  if (ret_code != 0) return ret_code;

  ret_code = update_output_buf(state);
 
  return ret_code;
}

