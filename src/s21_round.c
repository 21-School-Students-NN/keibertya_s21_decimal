#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"

int s21_round(s21_decimal value, s21_decimal *result) {
  int error = S21_ERROR;
  int scale = _get_scale(&value);
  if (result && scale <= 28) {
    error = S21_SUCCESS;
    *result = value;
    int reminder = 0;
    for (int i = 0; i < scale; i++) reminder = _divide_by_10(result, 0);
    _set_scale(result, 0);
    if (reminder > 5 || (reminder == 5 && (result->bits[0] & 1))) {
      s21_decimal one = {{1, 0, 0, 0}};
      _set_sign(&one, _get_sign(result));
      error = s21_add(*result, one, result);
    }
  }
  return error;
}