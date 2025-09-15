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
    *result = value;
    if (scale) {
      _set_sign(result, 0);
      _set_scale(result, MAX_SCALE);

      s21_decimal tmp = {{5, 0, 0, 0}};
      _set_scale(&tmp, MAX_SCALE);

      if (s21_sub(*result, tmp, result)) {
        error = S21_ERROR;
      }

      s21_decimal divider = ten_pows[scale];

      if (!error && s21_div(*result, divider, result)) {
        error = S21_ERROR;
      }

      _set_sign(result, _get_sign(&value));
      _set_scale(result, 0);
    }
  }

  return error;
}