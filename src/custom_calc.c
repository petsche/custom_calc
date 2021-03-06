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
string_copy(char* dest, const char* source) {
  /* Assumes source is null-terminated, avoids include */
  const char* end = source + CALC_IO_WIDTH;
  while (*source != 0 && source < end) {
    *dest = *source;
    ++dest;
    ++source;
  }
  *(dest) = 0;
}

custom_calc_status
update_output_buf(custom_calc_state* state) {
  custom_calc_status ret_code = CALC_STATUS_SUCCESS;
  if (state->input_size > 0) {
    string_copy(state->output_buf, state->input_buf);
  } else if (state->rpn_stack_size > 0) {
    ret_code =
      format_number_user(state->rpn_stack[state->rpn_stack_size - 1],
                         state->output_buf,
                         CALC_IO_WIDTH);
  } else {
    state->output_buf[0] = '0';
    state->output_buf[1] = 0;
  }
  if (ret_code == CALC_STATUS_SUCCESS) {
    /* Right justify */
    char* end = state->output_buf + CALC_IO_WIDTH;
    char* last_char = state->output_buf;
    while (*(last_char + 1) != 0 && last_char != end) {
      ++last_char;
    }
    if (last_char == end) {
      return CALC_STATUS_BUG;
    }
    *end = 0;
    --end;
    while (last_char >= state->output_buf) {
      *end = *last_char;
      --end;
      --last_char;
    }
    while (end >= state->output_buf) {
      *end = ' ';
      --end;
    }
  }
  return ret_code;
}

void reset_input_buf(custom_calc_state* state) {
  state->input_buf[0] = 0;
  state->input_size = 0;
}

custom_calc_status
custom_calc_init(custom_calc_state* state,
                 custom_calc_mode mode) {
  reset_input_buf(state);
  state->rpn_stack_size = 0;
  state->mode = mode;
  state->operator_stack_size = 0;
  state->last_operator = CALC_KEY_UNKNOWN;
  return update_output_buf(state);
}

void pop_input_char(custom_calc_state* state) {
  state->input_buf[state->input_size] = 0;
  --(state->input_size);
}

void push_input_char(custom_calc_state* state,
                     char c) {
  state->input_buf[state->input_size] = c;
  ++(state->input_size);
  state->input_buf[state->input_size] = 0;
}

custom_calc_status
process_number_input(custom_calc_state* state,
                     custom_calc_key key) {
  if (key == CALC_KEY_CLEAR_ENTRY) {
    reset_input_buf(state);
    push_input_char(state, '0');
    return CALC_STATUS_SUCCESS;
  } else if (key == CALC_KEY_BACKSPACE) {
    if (state->input_size > 0) {
      --(state->input_size);
      state->input_buf[state->input_size] = 0;
    } /* else ignore */
    if (state->input_size == 0) {
      push_input_char(state, '0');
    }
    return CALC_STATUS_SUCCESS;
  } else {
    if (state->input_size == CALC_IO_WIDTH) {
      /* Ignore if at limit */
      return CALC_STATUS_SUCCESS;
    }
    if (state->input_size == 1 &&
        state->input_buf[0] == '0') {
      /* Ignore 0 from previously cleared input */
      pop_input_char(state);
    }
    if (key == CALC_KEY_DECIMAL &&
      state->input_size == 0) {
      push_input_char(state, '0');
    }
    push_input_char(state, (char)key);
    return CALC_STATUS_SUCCESS;
  }
}

custom_calc_status
push_input_buf(custom_calc_state* state) {
  if (state->rpn_stack_size == CALC_MAX_STACK_SIZE) {
    return CALC_STATUS_STACK_OVERFLOW;
  }

  custom_calc_status ret_code =
    parse_number_user(state->input_buf,
                      &(state->rpn_stack[state->rpn_stack_size]));

  if (ret_code == CALC_STATUS_SUCCESS) {
    state->input_buf[0] = 0;
    state->input_size = 0;
    ++(state->rpn_stack_size);
  }
  return ret_code;
}

custom_calc_status
apply_operator_rpn(custom_calc_state* state, custom_calc_key op) {
  if (state->rpn_stack_size < 2) {
    return CALC_STATUS_INVALID_OPERATION;
  }

  custom_calc_status ret_code = CALC_STATUS_SUCCESS;
  CALC_NUMBER_TYPE left = state->rpn_stack[state->rpn_stack_size - 2];
  CALC_NUMBER_TYPE right = state->rpn_stack[state->rpn_stack_size - 1];
  CALC_NUMBER_TYPE op_output;
  ret_code = binary_operator_user(op, left, right, &op_output);
  if (ret_code == CALC_STATUS_SUCCESS) {
      /* Pop two values and push new value */
      state->rpn_stack[state->rpn_stack_size - 2] = op_output;
      --(state->rpn_stack_size);
      state->last_operator = op;
      state->last_right = right;
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
  custom_calc_status ret_code = CALC_STATUS_SUCCESS;
  while (state->operator_stack_size > 0 &&
         operator_precedence(operator_stack_top(state))
         >= operator_precedence(key)) {
    ret_code = apply_operator_rpn(state,
                            operator_stack_pop(state));
    if (ret_code != CALC_STATUS_SUCCESS) break;
  }
  if (state->operator_stack_size == CALC_MAX_STACK_SIZE) {
    ret_code = CALC_STATUS_STACK_OVERFLOW;
  }
  if (ret_code == CALC_STATUS_SUCCESS) {
    state->operator_stack[state->operator_stack_size] = key;
    ++(state->operator_stack_size);
  }
  return ret_code;
}

custom_calc_status
process_operator(custom_calc_state* state, custom_calc_key key) {
  custom_calc_status ret_code = CALC_STATUS_SUCCESS;
  if (key == CALC_KEY_FLIP_SIGN) {
    if (state->rpn_stack_size > 0) {
      return unary_operator_user(
               CALC_KEY_FLIP_SIGN,
               state->rpn_stack[state->rpn_stack_size - 1],
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
        if (state->operator_stack_size == 0
            && state->last_operator != CALC_KEY_UNKNOWN) {
          /* Push last right and operator for repeated equals behavior */
          if (state->rpn_stack_size == CALC_MAX_STACK_SIZE) {
            return CALC_STATUS_STACK_OVERFLOW;
          }
          state->rpn_stack[state->rpn_stack_size] = state->last_right;
          ++(state->rpn_stack_size);
          return apply_operator_rpn(state, state->last_operator);
        }
        while (state->operator_stack_size > 0) {
          ret_code = apply_operator_rpn(state, operator_stack_pop(state));
          if (ret_code != CALC_STATUS_SUCCESS) break;
        }
      } else {
        ret_code = push_infix_operator(state, key);
      }
      break;
    default:
      return CALC_STATUS_UNSUPPORTED_MODE;
  }
  return ret_code;
}

custom_calc_status
custom_calc_update(custom_calc_state* state, custom_calc_key key) {
  custom_calc_status ret_code = CALC_STATUS_SUCCESS;
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
      if (ret_code != CALC_STATUS_SUCCESS) break;
      ret_code = process_operator(state, key);
      break;
    default:
      ret_code = CALC_STATUS_UNSUPPORTED_KEY;
  }
  if (ret_code != CALC_STATUS_SUCCESS) return ret_code;

  ret_code = update_output_buf(state);
 
  return ret_code;
}

