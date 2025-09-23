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
    if (scale > 0) {
      _set_scale(result, 0);
      for (int i = 0; i < scale; i++) _divide_by_10(result, 0);
    }
  }
  return error;
}