#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  if (!result) {
    return S21_ERROR;
  }
  _init_decimal_zero(result);
  int error = S21_SUCCESS;
  meta_t scale = _get_scale(&value);
  if (scale > 28) {
    error = S21_ERROR;
  }
  if (!error) {
    if (scale == 0) {
      *result = value;
    } else {
      s21_decimal integer_part;
      _init_decimal_zero(&integer_part);

      meta_t sign = _get_sign(&value);

      _set_sign(&value, 0);

      if (s21_truncate(value, &integer_part)) {
        error = S21_ERROR;
      }

      if (!error && sign == 1 && !s21_is_equal(integer_part, value)) {
        s21_decimal one = {{1u, 0u, 0u, 0u}};
        if (s21_add(integer_part, one, result)) {
          error = S21_ERROR;
        }
      } else if (!error) {
        *result = integer_part;
      }

      if (!error) {
        _set_sign(result, sign);
        _set_scale(result, 0);
      }
    }
  }
  return error;
}