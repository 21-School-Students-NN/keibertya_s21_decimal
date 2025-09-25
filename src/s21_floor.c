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
      if (s21_truncate(value, result)) {
        error = S21_ERROR;
      } else if (_get_sign(&value) && !s21_is_equal(*result, value)) {
        s21_decimal one = {{1u, 0u, 0u, 0u}};
        if (s21_sub(*result, one, result)) {
          error = S21_ERROR;
        }
      }
    }
  }
  return error;
}