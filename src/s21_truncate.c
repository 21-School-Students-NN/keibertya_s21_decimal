#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
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
      s21_decimal divider = ten_pows[scale];

      if (s21_div(value, divider, result)) {
        error = S21_ERROR;
      } else {
        _set_sign(result, _get_sign(&value));
        _set_scale(result, 0);
      }
    }
  }

  return error;
}