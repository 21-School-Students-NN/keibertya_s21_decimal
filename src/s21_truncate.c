#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int error = S21_ERROR;
  int scale = _get_scale(&value);
  if (result && scale <= 28) {
    *result = value;
    if (scale > 0)
      for (int i = 0; i < scale; i++) _divide_by_10(result, 0);
    _set_scale(result, 0);
    error = S21_SUCCESS;
  }
  return error;
}